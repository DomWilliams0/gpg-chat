#include <openssl/ssl.h>
#include <openssl/err.h>
#include <unistd.h>
#include <stdio.h>
#include "shared_utils.h"

const SSL_METHOD* (*SERVER_SSL_METHOD)(void) = TLSv1_2_server_method;
const SSL_METHOD* (*CLIENT_SSL_METHOD)(void) = TLSv1_2_client_method;
 
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

int parse_port(char *s)
{
	int i;
	i = strtol(s, NULL, 10);
	if (i < 1 || i > 65535)
		return ERROR_BAD_INPUT;

	return i;
}
