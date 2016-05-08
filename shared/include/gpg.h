#ifndef GPGCHAT_GPG_H
#define GPGCHAT_GPG_H

#include <gpgme.h>
#include <stdbool.h>

typedef struct gpg_context GPG_CTX;

GPG_CTX *GPG_CTX_new();

void GPG_CTX_free(GPG_CTX *ctx);

bool GPG_get_key(GPG_CTX *ctx, char *search_term, gpgme_key_t *out);

bool GPG_encrypt(GPG_CTX *ctx, gpgme_key_t recp[], gpgme_data_t plaintext, gpgme_data_t out, bool sign);

bool GPG_decrypt(GPG_CTX *ctx, gpgme_data_t ciphertext, gpgme_data_t plaintext);
#endif
