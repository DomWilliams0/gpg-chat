#ifndef GPGCHAT_CLIENT_SOCKET_H
#define GPGCHAT_CLIENT_SOCKET_H
#include <openssl/ssl.h>

int create_socket(char *host, unsigned short port);

void init_ssl(SSL_CTX **ctx, SSL **ssl);

#endif
