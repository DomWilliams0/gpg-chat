#ifndef GPGCHAT_SERVER_PARSER_H
#define GPGCHAT_SERVER_PARSER_H
#include <stdbool.h>

struct server_settings
{
	unsigned short port;
	char *cert_path;
	char *key_path;

	bool valid;
};

int parse_server_settings(int argc, char **argv, struct server_settings *out);

#endif

