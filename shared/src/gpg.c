#include <stdlib.h>
#include <gpgme.h>
#include <string.h>
#include "gpg.h"
#include "shared_utils.h"

void gpg_error_check(gpgme_error_t err, const char *msg)
{
	// success
	if (gpg_err_code(err) != GPG_ERR_NO_ERROR)
	{
		fprintf(stderr, "\n** %s %s: %s\n", msg, gpgme_strsource(err), gpgme_strerror(err));
		// TODO do not crash
		exit(ERROR_GPGME);
	}
}

struct gpg_context
{
	gpgme_ctx_t ctx;
};


GPG_CTX *GPG_CTX_new()
{
	GPG_CTX *ctx = calloc(1, sizeof(GPG_CTX));
	if (ctx == NULL)
		error("Failed to allocate GPG_CTX");

	gpgme_error_t err;
	const char *version;

	version = gpgme_check_version(NULL);
	printf("GPGme version: %s\n", version);

	err = gpgme_new(&ctx->ctx);
	gpg_error_check(err, "Failed to create GPGme context");

	gpgme_set_armor(ctx->ctx, 1);

	return ctx;
}

void GPG_CTX_free(GPG_CTX *ctx)
{
	if (ctx == NULL)
		return;

	// clean up
	gpgme_release(ctx->ctx);
}


bool GPG_get_key(GPG_CTX *ctx, char *search_term, gpgme_key_t *out)
{
	gpg_error_t err;
	gpgme_key_t key;
	gpgme_keylist_mode_t search_mode;
	
	search_mode	= GPGME_KEYLIST_MODE_LOCAL;
	gpgme_set_keylist_mode(ctx->ctx, search_mode);
	gpgme_set_protocol(ctx->ctx, GPGME_PROTOCOL_OpenPGP);

	gpg_error_check(gpgme_op_keylist_start(ctx->ctx, search_term, 0), "Failed to start searching keylist");

	err = gpgme_op_keylist_next(ctx->ctx, &key);

	gpgme_err_code_t err_code = gpg_err_code(err);

	if (err_code != GPG_ERR_EOF)
		gpg_error_check(err, "Failed to get key");

	gpgme_op_keylist_end(ctx->ctx);

	if (err_code == GPG_ERR_NO_ERROR)
	{
		*out = key;
		return true;
	}

	return false;
}

bool GPG_encrypt(GPG_CTX *ctx, gpgme_key_t recp[], gpgme_data_t plaintext, gpgme_data_t out, bool sign)
{
	gpgme_error_t err;
	gpgme_err_code_t err_code;
	gpgme_error_t (*op)(gpgme_ctx_t, gpgme_key_t[], gpgme_encrypt_flags_t, gpgme_data_t, gpgme_data_t);

	op = sign ? gpgme_op_encrypt_sign : gpgme_op_encrypt;
	err = op(ctx->ctx, recp, 0, plaintext, out);
	err_code = gpg_err_code(err);

	// invalid recipients
	if (err_code == GPG_ERR_UNUSABLE_PUBKEY)
	{
		gpgme_encrypt_result_t res = gpgme_op_encrypt_result(ctx->ctx);
		gpgme_invalid_key_t bad_key = res->invalid_recipients;

		while(bad_key != NULL)
		{
			printf("** Bad recipient %s: %s\n", bad_key->fpr, gpgme_strsource(bad_key->reason));
			bad_key = bad_key->next;
		}

		return false;
	}

	gpg_error_check(err, "Failed to encrypt message");

	// rewind
	ssize_t n = gpgme_data_seek(out, 0, SEEK_SET);
	if (n < 0)
		error("Failed to rewind ciphertext buffer");

	return true;
}

bool GPG_decrypt(GPG_CTX *ctx, gpgme_data_t ciphertext, gpgme_data_t plaintext)
{
	gpgme_error_t err;

	err = gpgme_op_decrypt(ctx->ctx, ciphertext, plaintext);
	gpg_error_check(err, "Failed to decrypt");

	gpgme_data_seek(plaintext, 0, SEEK_SET);

	return true;
}
