#include <openssl/bio.h>
#include <openssl/err.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include "socket.h"
#include "shared_utils.h"
#include "client_parser.h"
#include "gpg.h"

int create_socket(char *host, unsigned short port, SSL *ssl)
{
	int s;

	if ((s = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		error_ret("Failed to create socket\n", -1);

	struct sockaddr_in addr;
	struct hostent *server;

	// lookup host
	server = gethostbyname(host);
	if (server == NULL)
		error_ret("Invalid host\n", -1);

	bzero((char *) &addr, sizeof addr);
	addr.sin_family = AF_INET;
	bcopy((char *) server->h_addr, (char *) &addr.sin_addr.s_addr, server->h_length);
	addr.sin_port = htons(port);

	// connect
	if (connect(s, (struct sockaddr *) &addr, sizeof addr) < 0)
		error_ret("Failed to connect socket\n", -1);

	SSL_set_fd(ssl, s);
	if (SSL_connect(ssl) != 1)
		error_ret("Failed to connect with SSL\n", -1);

	return s;
}

int init_ssl(SSL_CTX **ctx, SSL **ssl)
{
	SSL_load_error_strings();
	SSL_library_init();
	OpenSSL_add_all_algorithms();

	if ((*ctx = SSL_CTX_new(CLIENT_SSL_METHOD())) == NULL)
		error_ret("Failed to create SSL context\n", ERROR_OPENSSL);

	SSL_CTX_set_options(*ctx, SSL_OP_NO_SSLv2);

	*ssl = SSL_new(*ctx);
	return ERROR_NO_ERROR;
}
