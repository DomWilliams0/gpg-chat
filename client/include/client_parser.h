#ifndef GPGCHAT_CLIENT_PARSER_H
#define GPGCHAT_CLIENT_PARSER_H

struct client_settings
{
	unsigned short host_port;
	char *host;
};

void parse_client_settings(int argc, char **argv, struct client_settings *out);

#endif

