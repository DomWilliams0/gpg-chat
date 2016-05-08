#include <stdlib.h>
#include <gpgme.h>
#include "gpg.h"
#include "shared_utils.h"

void gpg_error_check(gpgme_error_t err, const char *msg)
{
	// success
	if (err != GPG_ERR_NO_ERROR)
	{
		fprintf(stderr, "\n** %s: %d %s %s: %s\n", __FILE__, __LINE__, msg, gpgme_strsource(err), gpgme_strerror(err));
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
