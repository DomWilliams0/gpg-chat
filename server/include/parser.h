#ifndef GPGCHAT_PARSER_H
#define GPGCHAT_PARSER_H

struct user_settings
{
	unsigned short port;
	const char *cert_path;
	const char *key_path;
};

void parse_settings(int argv, char **argc, struct user_settings *out);

#endif

