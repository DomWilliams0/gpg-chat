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

int create_socket(char *host, unsigned short port)
{
	int s;

	if ((s = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		error("Failed to create socket");

	struct sockaddr_in addr;
	struct hostent *server;

	// lookup host
	server = gethostbyname(host);
	if (server == NULL)
		error("Invalid host");

	bzero((char *) &addr, sizeof addr);
	addr.sin_family = AF_INET;
	bcopy((char *) server->h_addr, (char *) &addr.sin_addr.s_addr, server->h_length);
	addr.sin_port = htons(port);

	// connect
	if (connect(s, (struct sockaddr *) &addr, sizeof addr) < 0)
		error("Failed to connect socket");

	return s;
}

void init_ssl(SSL_CTX **ctx, SSL **ssl)
{
	SSL_load_error_strings();
	SSL_library_init();
	OpenSSL_add_all_algorithms();

	if ((*ctx = SSL_CTX_new(CLIENT_SSL_METHOD())) == NULL)
		error("Failed to create SSL context");

	SSL_CTX_set_options(*ctx, SSL_OP_NO_SSLv2);

	*ssl = SSL_new(*ctx);
}

/* void example_usage() */
/* { */
/*  	int sock; */
/*  	SSL_CTX *ssl_ctx; */
/*  	SSL *ssl; */

/*  	// init openssl and socket */
/*  	init_ssl(&ssl_ctx, &ssl); */
/* 	sock = create_socket(settings.host, settings.host_port); */

/* 	// connect */
/* 	SSL_set_fd(ssl, sock); */
/* 	if (SSL_connect(ssl) != 1) */
/* 		error("Failed to connect with SSL"); */

/* 	char msg[] = "test message from the client over ssl!\n"; */
/* 	int n = SSL_write(ssl, msg, sizeof msg); */
/* 	printf("wrote %d bytes\n", n); */

/* 	close(sock); */
/* } */

