#ifndef GPGCHAT_SHARED_UTILS_H
#define GPGCHAT_SHARED_UTILS_H

#include <stdbool.h>
#include <stdlib.h>

#define error(msg) handle_error(__FILE__, __LINE__, msg)

void handle_error(const char* file, int lineno, const char* msg);

bool file_exists(const char *path);

#endif