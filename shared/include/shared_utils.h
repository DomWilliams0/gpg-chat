#ifndef GPGCHAT_SHARED_UTILS_H
#define GPGCHAT_SHARED_UTILS_H

#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>

// Portable 64 bit host-to-network conversion
// Merci http://stackoverflow.com/a/4410728
#if defined(__linux__)
#  include <endian.h>
#elif defined(__FreeBSD__) || defined(__NetBSD__)
#  include <sys/endian.h>
#elif defined(__OpenBSD__)
#  include <sys/types.h>
#  define be16toh(x) betoh16(x)
#  define be32toh(x) betoh32(x)
#  define be64toh(x) betoh64(x)
#endif

// Prints the given error message to stderr
#define error_print(msg) fprintf(stderr, "ERROR: %s", msg);

// Prints the given error message and returns
// with the given value
#define error_ret(msg, ret) {\
		error_print(msg)\
		return ret;}

// Prints the given error message and
// invalidates the local settings struct
#define parse_error(msg) {\
	error_print(msg);\
	settings->valid = false;\
	}

#define print_usage puts("Try `"EXECUTABLE_NAME" --help' or `"EXECUTABLE_NAME" --usage' for more information.")

// Error codes
#define ERROR_NO_ERROR		0
#define ERROR_BAD_INPUT   1
#define ERROR_GPGME       2
#define ERROR_SOCKET      3
#define ERROR_OPENSSL     4
#define ERROR_USER_ABORT  5
#define ERROR_BAD_OP      6

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

int parse_port(char *s);

#endif
