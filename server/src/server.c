#include <openssl/bio.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <openssl/ec.h>
#include <netdb.h>
#include <unistd.h>
#include <linux/limits.h>
#include <argp.h>
#include <signal.h>
#include "shared_utils.h"
#include "server_parser.h"
#include "socket.h"
#include "server_actions.h"

static volatile bool server_running = true;

void handle_interrupt(int x);

/**
 * Cleanly breaks the server loop on ^C
 */
void register_signal_handler();

int accept_ssl(SSL_CTX *ssl_ctx, SSL **ssl, int client);

int main(int argc, char **argv)
{
	int ret;
	struct server_settings settings;

	// load arguments
	ret = parse_server_settings(argc, (char **) argv, &settings);

	if (is_failure(ret))
	{
		print_usage;
		return ret;
	}

	int sock;
	SSL_CTX *ssl_ctx;
	SSL *ssl;
	char op[OP_LENGTH];

	// init ssl
	if (is_failure(init_ssl(&ssl_ctx, NULL, settings.cert_path, settings.key_path)))
		goto GENERAL_CLEAN_UP;

	sock = create_server_socket(settings.port);

	printf("Listening on port %d\n", settings.port);
	register_signal_handler();

	while (server_running)
	{
		struct sockaddr_in addr;
		uint len = sizeof addr;

		puts("Waiting for connections...");
		int client = accept(sock, (struct sockaddr *) &addr, &len);
		if (client < 0)
		{
			// not clean
			if (server_running)
			{
				error_print("Failed to accept client connection\n");
				ret = ERROR_SOCKET;
			}

			break;
		}

		puts("Client connected");
		if (is_failure(accept_ssl(ssl_ctx, &ssl, client)))
			continue;

		// read op code and execute action
		SSL_read(ssl, op, OP_LENGTH);

		ret = handle_server_action(&settings, ssl, op);

		// TODO handle error
		if (is_failure(ret))
			error_print("Failed to handle client action\n");

		// close connection
		SSL_free(ssl);
		close(client);
		puts("Closed connection");
	}


GENERAL_CLEAN_UP:
	puts("\nCleaning up");

	if (ssl_ctx != NULL)
		SSL_CTX_free(ssl_ctx);

	ERR_free_strings();
	EVP_cleanup();

	return ret;
}

void handle_interrupt(int x)
{
	UNUSED(x);
	server_running = false;
}

/**
 * Cleanly breaks the server loop on ^C
 */
void register_signal_handler()
{
	struct sigaction act;
	act.sa_handler = handle_interrupt;
	sigaction(SIGINT, &act, NULL);
}

int accept_ssl(SSL_CTX *ssl_ctx, SSL **ssl, int client)
{
	*ssl = SSL_new(ssl_ctx);
	SSL_set_fd(*ssl, client);

	if (SSL_accept(*ssl) <= 0)
	{
		ERR_print_errors_fp(stderr);
		SSL_free(*ssl);
		close(client);
		return ERROR_SOCKET;
	}

	return ERROR_NO_ERROR;
}
