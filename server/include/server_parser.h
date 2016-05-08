#ifndef GPGCHAT_SERVER_PARSER_H
#define GPGCHAT_SERVER_PARSER_H

struct server_settings
{
	unsigned short port;
	const char *cert_path;
	const char *key_path;
};

void parse_server_settings(int argc, char **argv, struct server_settings *out);

#endif

