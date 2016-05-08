#ifndef GPGCHAT_GPG_H
#define GPGCHAT_GPG_H

#include <gpgme.h>

typedef struct gpg_context GPG_CTX;

GPG_CTX *GPG_CTX_new();

void GPG_CTX_free(GPG_CTX *ctx);

/**
 * Returns GPG_ERR_NO_ERROR on success
 */
gpgme_err_code_t GPG_get_key(GPG_CTX *ctx, char *search_term, gpgme_key_t *out);


#endif
