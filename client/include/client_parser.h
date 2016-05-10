#ifndef GPGCHAT_CLIENT_PARSER_H
#define GPGCHAT_CLIENT_PARSER_H
#include <stdbool.h>

struct client_settings;

int parse_client_settings(int argc, char **argv, struct client_settings *out);

void print_usage();

#endif
