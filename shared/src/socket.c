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

static int create_base_socket(unsigned short port, struct sockaddr_in *addr)
{
	int s;

	if ((s = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		error_ret("Failed to create socket\n", ERROR_SOCKET);

	bzero((char *) addr, sizeof *addr);
	addr->sin_family = AF_INET;
	addr->sin_port = htons(port);

	return s;
}

int create_client_socket(char *host, unsigned short port, SSL *ssl)
{
	int s;
	struct sockaddr_in addr;
	struct hostent *server;

	if ((s = create_base_socket(port, &addr)) < 0)
		return s;

	// lookup host
	server = gethostbyname(host);
	if (server == NULL)
		error_ret("Invalid host\n", -1);

	bcopy((char *) server->h_addr, (char *) &addr.sin_addr.s_addr, server->h_length);

	// connect
	if (connect(s, (struct sockaddr *) &addr, sizeof addr) < 0)
		error_ret("Failed to connect socket\n", -1);

	SSL_set_fd(ssl, s);
	if (SSL_connect(ssl) != 1)
		error_ret("Failed to connect with SSL\n", -1);

	return s;
}

int create_server_socket(unsigned short port)
{
	int s;
	struct sockaddr_in addr;

	if ((s = create_base_socket(port, &addr)) < 0)
		return s;

	addr.sin_addr.s_addr = INADDR_ANY;

	if (bind(s, (struct sockaddr *) &addr, sizeof(addr)) < 0)
		error_ret("Failed to bind socket\n", ERROR_SOCKET);

	if (listen(s, 1) < 0)
		error_ret("Failed to listen\n", ERROR_SOCKET);

	return s;
}

int init_ssl(SSL_CTX **ctx, SSL **ssl, const char *server_cert, const char *server_key)
{
	SSL_load_error_strings();
	SSL_library_init();
	OpenSSL_add_all_algorithms();

	bool is_server = server_cert != NULL && server_key != NULL;

	if ((*ctx = SSL_CTX_new(is_server ? SERVER_SSL_METHOD() : CLIENT_SSL_METHOD())) == NULL)
		error_ret("Failed to create SSL context\n", ERROR_OPENSSL);

	SSL_CTX_set_ecdh_auto(*ctx, 1);
	SSL_CTX_set_options(*ctx, SSL_OP_NO_SSLv2);

	// server specific
	if (is_server)
	{
		if (!file_exists(server_cert) ||
				SSL_CTX_use_certificate_file(*ctx, server_cert, SSL_FILETYPE_PEM) < 0)
			error_ret("Failed to load certificate\n", ERROR_OPENSSL);

		if (!file_exists(server_key) ||
				SSL_CTX_use_PrivateKey_file(*ctx, server_key, SSL_FILETYPE_PEM) < 0)
			error_ret("Failed to load key\n", ERROR_OPENSSL);

	}

	// client specific
	else
	{
		*ssl = SSL_new(*ctx);
		if (*ssl == NULL)
			error_ret("Failed to create SSL structure\n", ERROR_OPENSSL);
	}

	return ERROR_NO_ERROR;
}
