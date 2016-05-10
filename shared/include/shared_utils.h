#ifndef GPGCHAT_SHARED_UTILS_H
#define GPGCHAT_SHARED_UTILS_H

#include <stdbool.h>
#include <stdlib.h>

// Prints the given error message to stderr
#define error_print(msg) fprintf(stderr, "ERROR: %s", msg);

// Prints the given error message and returns
// with the given value
#define error_ret(msg, ret) {\
		error_print(msg)\
		return ret;}

// Error codes
#define ERROR_NO_ERROR		0
#define ERROR_BAD_INPUT   1
#define ERROR_GPGME       2
#define ERROR_SOCKET      3
#define ERROR_OPENSSL     4
#define ERROR_USER_ABORT  5

// Checks if the given error code is not a success
#define is_failure(x) (x != ERROR_NO_ERROR)

// Declares a variable as unused, and calms
// down -pedantic
#define UNUSED(x) (void)(x)

// Constants
#define DEFAULT_PORT      10800
#define DEFAULT_PORT_STR  "10800"

// SSL method shared by client and server
extern const struct ssl_method_st* (*SERVER_SSL_METHOD)(void);
extern const struct ssl_method_st* (*CLIENT_SSL_METHOD)(void);

bool file_exists(const char *path);

bool request_confirmation(const char *msg);

#endif
