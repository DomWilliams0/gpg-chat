#include <argp.h>
#include "client_actions.h"
#include "client_parser.h"
#include "shared_utils.h"

int handle_action(struct client_settings *settings, GPG_CTX *gpg_ctx)
{
	// global validation
	if (!settings->host)
		error_ret("Host required\n", ERROR_BAD_INPUT);

	// TODO parse host:port instead of being given separately

	int (*action)(struct client_settings *, GPG_CTX *) = NULL;
	switch(settings->action)
	{
		case CONNECT:
			action = do_action_connect;
			break;
		case MESSAGE:
			action = do_action_message;
			break;
		default:
			error_ret("Unimplemented action\n", ERROR_BAD_INPUT);
	}

	return action(settings, gpg_ctx);
}
