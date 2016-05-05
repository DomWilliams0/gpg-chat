#include <openssl/bio.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <openssl/ec.h>
#include <netdb.h>
#include <unistd.h>

#define error(msg) handle_error(__FILE__, __LINE__, msg)

void handle_error(const char* file, int lineno, const char* msg)
{
	printf("\n** %s: %d %s\n", file, lineno, msg);
	ERR_print_errors_fp(stderr);
	exit(-1);
}

int create_socket(int port)
{
	int s;

	if ((s = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		error("Failed to create socket");

	struct sockaddr_in addr;
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
	int port = 11500;
	int sock = create_socket(port);
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
		// TODO ...
	}

}
