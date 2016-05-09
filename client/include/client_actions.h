#ifndef GPGCHAT_CLIENT_ACTIONS_H
#define GPGCHAT_CLIENT_ACTIONS_H
#include "gpg.h"
#include "client_parser.h"

enum client_action { REGISTER, MESSAGE };

struct client_settings
{
	enum client_action action;
	unsigned short host_port;
	char *host;

	// messaging
	char **recipients;
	bool sign_message;

	// registering
	char *key;
};


void handle_action(struct client_settings *settings, GPG_CTX *gpg_ctx);

void do_action_register(struct client_settings *settings, GPG_CTX *gpg_ctx);

void do_action_message(struct client_settings *settings, GPG_CTX *gpg_ctx);

#endif

