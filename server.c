#include "shared_utils.h"
#include <openssl/bio.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <openssl/ec.h>
#include <netdb.h>
#include <unistd.h>

SSL_CTX *create_ssl_context(const char *cert, const char *key) 
{
	SSL_CTX *ctx;
	const SSL_METHOD *method;

	method = SSLv23_server_method();

	if ((ctx = SSL_CTX_new(method)) == NULL)
		error("Failed to create SSL context");

	SSL_CTX_set_ecdh_auto(ctx, 1);

	if (!file_exists(cert) || SSL_CTX_use_certificate_file(ctx, cert, SSL_FILETYPE_PEM) < 0)
		error("Failed to load certificate");

	if (!file_exists(key) || SSL_CTX_use_PrivateKey_file(ctx, key, SSL_FILETYPE_PEM) < 0)
		error("Failed to load key");

	return ctx;
}

int create_socket(int port)
{
	int s;

	if ((s = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		error("Failed to create socket");

	struct sockaddr_in addr;
	bzero((char *) &addr, sizeof addr);
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = htons(port);

	if (bind(s, (struct sockaddr *) &addr, sizeof(addr)) < 0)
		error("Failed to bind socket");

	if (listen(s, 1) < 0)
		error("Failed to listen");

	return s;
}

int main()
{
	// TODO: program args
	int port = 11500;
	const char *certPath = "/etc/test-certs/cert.pem";
	const char *keyPath = "/etc/test-certs/key.pem";

	// init openssl
	SSL_load_error_strings();
	SSL_library_init();
	OpenSSL_add_all_algorithms();

	int sock;
	SSL_CTX *ssl_ctx;
	SSL *ssl;

	ssl_ctx = create_ssl_context(certPath, keyPath);
	sock = create_socket(port);

	printf("Listening on port %d\n", port);

	while (1)
	{
		struct sockaddr_in addr;
		uint len = sizeof addr;

		puts("Waiting for connections...");
		int client = accept(sock, (struct sockaddr *) &addr, &len);
		if (client < 0)
			error("Failed to accept connection");

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
	ERR_free_strings();
	EVP_cleanup();
	SSL_shutdown(ssl);
	SSL_free(ssl);

}
