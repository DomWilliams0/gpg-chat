#ifndef GPGCHAT_CLIENT_PARSER_H
#define GPGCHAT_CLIENT_PARSER_H
#include <stdbool.h>

enum client_action { REGISTER, MESSAGE };

struct client_settings
{
	enum client_action action;
	char **recipients;
	unsigned short host_port;
	bool sign_message;
	char *host;
};

void parse_client_settings(int argc, char **argv, struct client_settings *out);

#endif

