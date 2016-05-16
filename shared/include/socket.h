#ifndef GPGCHAT_CLIENT_SOCKET_H
#define GPGCHAT_CLIENT_SOCKET_H
#include <openssl/ssl.h>

#define OP_LENGTH     3
#define OP_CONNECT    "CON"
#define OP_MESSAGE    "MSG"
#define OP_REQUEST    "REQ"

#define PACKET_SIZE   1024

/**
 * Returns socket fd, or <0 if the operation failed
 */
int create_client_socket(char *host, unsigned short port, SSL *ssl);

int create_server_socket(unsigned short port);

/**
 * For server, set ssl to NULL
 * For client, set cert and key paths to NULL
 */
int init_ssl(SSL_CTX **ctx, SSL **ssl, const char *server_cert, const char *server_key);

#endif
