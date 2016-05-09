#include <argp.h>
#include "client_actions.h"
#include "client_parser.h"
#include "shared_utils.h"


void handle_action(struct client_settings *settings, GPG_CTX *gpg_ctx)
{
	// global validation
	if (!settings->host)
		error_argp("Host required\n");

	// TODO parse host:port instead of being given separately

	void (*action)(struct client_settings *, GPG_CTX *) = NULL;
	switch(settings->action)
	{
		case REGISTER:
			action = do_action_register;
			break;
		case MESSAGE:
			action = do_action_message;
			break;
		default:
			error_argp("Unimplemented action\n");
			return;
	}

	action(settings, gpg_ctx);
}
