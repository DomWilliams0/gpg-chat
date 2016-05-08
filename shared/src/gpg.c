#include <stdlib.h>
#include <gpgme.h>
#include "gpg.h"
#include "shared_utils.h"

void gpg_error_check(gpgme_error_t err, const char *msg)
{
	// success
	if (gpg_err_code(err) != GPG_ERR_NO_ERROR)
	{
		fprintf(stderr, "\n** %s %s: %s\n", msg, gpgme_strsource(err), gpgme_strerror(err));
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

	return ctx;
}

void GPG_CTX_free(GPG_CTX *ctx)
{
	if (ctx == NULL)
		return;

	// clean up
	gpgme_release(ctx->ctx);
}


gpgme_err_code_t GPG_get_key(GPG_CTX *ctx, char *search_term, gpgme_key_t *out)
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
		return GPG_ERR_NO_ERROR;
	}

	return GPG_ERR_EOF;
}
