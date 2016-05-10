#ifndef GPGCHAT_CLIENT_SOCKET_H
#define GPGCHAT_CLIENT_SOCKET_H
#include <openssl/ssl.h>

#define OP_LENGTH     3
#define OP_REGISTER   "REG"
#define OP_MESSAGE    "MSG"
#define OP_REQUEST    "REQ"

#define PACKET_SIZE   1024

int create_socket(char *host, unsigned short port, SSL *ssl);

void init_ssl(SSL_CTX **ctx, SSL **ssl);

#endif
