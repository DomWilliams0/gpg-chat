#include <openssl/err.h>
#include <unistd.h>
#include <stdio.h>
#include "shared_utils.h"

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
