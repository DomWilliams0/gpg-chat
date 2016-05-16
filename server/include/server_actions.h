#ifndef GPGCHAT_SERVER_ACTIONS_H
#define GPGCHAT_SERVER_ACTIONS_H
#include <openssl/ssl.h>

struct server_settings;

int handle_server_action(struct server_settings *settings, SSL *connection, char *op);

int do_server_action_connect(struct server_settings *settings, SSL *connection);

int do_server_action_message(struct server_settings *settings, SSL *connection);
#endif

