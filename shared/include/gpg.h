#ifndef GPGCHAT_GPG_H
#define GPGCHAT_GPG_H

#include <gpgme.h>

typedef struct gpg_context GPG_CTX;

GPG_CTX *GPG_CTX_new();

void GPG_CTX_free(GPG_CTX *ctx);




#endif
