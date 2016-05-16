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

static volatile bool server_running = true;

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

int main(int argc, char **argv)
{
	int ret;
	struct server_settings settings;

	// load arguments
	ret = parse_server_settings(argc, (char **) argv, &settings);

	if (ret != ERROR_NO_ERROR)
	{
		print_usage;
		return ret;
	}

	int sock;
	SSL_CTX *ssl_ctx;
	SSL *ssl;

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
				error_print("Failed to accept client connection\n");

			break;
		}

		puts("Client connected");
		ssl = SSL_new(ssl_ctx);
		SSL_set_fd(ssl, client);

		if (SSL_accept(ssl) <= 0)
		{
			ERR_print_errors_fp(stderr);
			SSL_free(ssl);
			close(client);
			continue;
		}

		// read all input and print
		char buf[256];
		int buf_size = sizeof buf;
		int byte_count = buf_size;
		puts("======== begin recv ========");
		while (true) 
		{
			memset(buf, '\0', byte_count);
			byte_count = SSL_read(ssl, buf, (sizeof buf));

			printf("%s", buf);

			// buffer not filled = end reached
			if (byte_count < buf_size)
				break;
		}
		puts("\n======== end recv ========");

		// simple reply
		SSL_write(ssl, "Hiya!\n", 6);

		// close connection
		SSL_free(ssl);
		close(client);
		puts("Closed connection");
	}

	// clean up
	SSL_CTX_free(ssl_ctx);

GENERAL_CLEAN_UP:
	puts("\nCleaning up");
	ERR_free_strings();
	EVP_cleanup();

	return 0;
}
