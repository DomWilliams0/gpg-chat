#include <openssl/ssl.h>
#include <openssl/err.h>
#include <unistd.h>
#include <stdio.h>
#include "shared_utils.h"

const SSL_METHOD* (*SERVER_SSL_METHOD)(void) = TLSv1_2_server_method;
const SSL_METHOD* (*CLIENT_SSL_METHOD)(void) = TLSv1_2_client_method;
 
void handle_error(const char* file, int lineno, const char* msg)
{
	printf("\n** %s: %d %s\n", file, lineno, msg);
	ERR_print_errors_fp(stderr);
	exit(-1);
}

bool file_exists(const char *path)
{
	return access(path, R_OK) == 0;
}

bool request_confirmation(const char *msg)
{
	printf("%s (y/N) ", msg);
	char choice = 'n';

	scanf("%c", &choice);
	return choice == 'y' || choice == 'Y';
}
