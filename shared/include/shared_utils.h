#ifndef GPGCHAT_SHARED_UTILS_H
#define GPGCHAT_SHARED_UTILS_H

#include <stdbool.h>
#include <stdlib.h>

#define error(msg) handle_error(__FILE__, __LINE__, msg)
#define UNUSED(x) (void)(x)

#define ERROR_BAD_INPUT 1
#define ERROR_GPGME 2

#define DEFAULT_PORT 10800
#define DEFAULT_PORT_STR "10800"

extern const struct ssl_method_st* (*SERVER_SSL_METHOD)(void);
extern const struct ssl_method_st* (*CLIENT_SSL_METHOD)(void);

void handle_error(const char* file, int lineno, const char* msg);

bool file_exists(const char *path);

#endif
