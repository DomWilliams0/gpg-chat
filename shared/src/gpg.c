#include <stdlib.h>
#include <gpgme.h>
#include <string.h>
#include "gpg.h"
#include "shared_utils.h"

// On error, prints the message and returns
// with the given code
#define gpg_error_check_ret(err, msg, ret_code){\
	if (gpg_err_code(err) != GPG_ERR_NO_ERROR)\
		error_ret(msg, ret_code);}

// On error, prints the message and returns
// with ERROR_GPGME
#define gpg_error_check(err, msg)\
	gpg_error_check_ret(err, msg, ERROR_GPGME);

struct gpg_context
{
	gpgme_ctx_t ctx;
};


int GPG_CTX_new(GPG_CTX **ctx)
{
	GPG_CTX *new_ctx;
	*ctx = NULL;

	new_ctx = calloc(1, sizeof(GPG_CTX));
	if (new_ctx == NULL)
		error_ret("Failed to allocate GPG_CTX\n", ERROR_GPGME);
	*ctx = new_ctx;

	gpgme_error_t err;
	const char *version;

	version = gpgme_check_version(NULL);
	printf("GPGme version: %s\n", version);

	err = gpgme_new(&new_ctx->ctx);
	gpg_error_check_ret(err, "Failed to create GPGme context\n", ERROR_GPGME);

	gpgme_set_armor(new_ctx->ctx, 1);

	return ERROR_NO_ERROR;
}

void GPG_CTX_free(GPG_CTX *ctx)
{
	if (ctx == NULL)
		return;

	// clean up
	gpgme_release(ctx->ctx);
}


int GPG_get_key(GPG_CTX *ctx, char *search_term, gpgme_key_t *out)
{
	gpg_error_t err;
	gpgme_key_t key;
	gpgme_keylist_mode_t search_mode;
	
	search_mode	= GPGME_KEYLIST_MODE_LOCAL;
	gpgme_set_keylist_mode(ctx->ctx, search_mode);
	gpgme_set_protocol(ctx->ctx, GPGME_PROTOCOL_OpenPGP);

	gpg_error_check(gpgme_op_keylist_start(ctx->ctx, search_term, 0), "Failed to start searching keylist\n");

	err = gpgme_op_keylist_next(ctx->ctx, &key);

	gpgme_err_code_t err_code = gpg_err_code(err);

	if (err_code != GPG_ERR_EOF)
		gpg_error_check(err, "Failed to get key\n");

	gpgme_op_keylist_end(ctx->ctx);

	if (err_code == GPG_ERR_NO_ERROR)
	{
		*out = key;
		return ERROR_NO_ERROR;
	}

	return ERROR_GPGME;
}

int GPG_encrypt(GPG_CTX *ctx, gpgme_key_t recp[], gpgme_data_t plaintext, gpgme_data_t out, bool sign)
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
			printf("Bad recipient %s: %s\n", bad_key->fpr, gpgme_strsource(bad_key->reason));
			bad_key = bad_key->next;
		}

		return ERROR_GPGME;
	}

	gpg_error_check(err, "Failed to encrypt message\n");

	// rewind
	ssize_t n = gpgme_data_seek(out, 0, SEEK_SET);
	if (n < 0)
		error_ret("Failed to rewind ciphertext buffer\n", ERROR_GPGME);

	return ERROR_NO_ERROR;
}

int GPG_decrypt(GPG_CTX *ctx, gpgme_data_t ciphertext, gpgme_data_t plaintext)
{
	gpgme_error_t err;

	err = gpgme_op_decrypt(ctx->ctx, ciphertext, plaintext);
	gpg_error_check(err, "Failed to decrypt\n");

	gpgme_data_seek(plaintext, 0, SEEK_SET);

	return ERROR_NO_ERROR;
}

int GPG_export(GPG_CTX *ctx, gpgme_key_t key, gpgme_data_t out)
{
	gpg_error_t err;

	gpgme_key_t keys[] = { key, NULL };

	// export key
	err = gpgme_op_export_keys(ctx->ctx, keys, 0, out);
	gpg_error_check(err, "Failed to export key\n");

	ssize_t n = gpgme_data_seek(out, 0, SEEK_SET);
	if (n < 0)
		error_ret("Failed to rewind\n", ERROR_GPGME);

	return ERROR_NO_ERROR;
}
