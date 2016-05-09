#include <argp.h>
#include "client_actions.h"
#include "client_parser.h"
#include "shared_utils.h"

void do_action_message(struct client_settings *settings, GPG_CTX *gpg_ctx)
{
	// validation
	if (settings->recipients == NULL)
		error_argp("Recipients required\n");

	UNUSED(gpg_ctx);
	puts("TODO: Messaging...");
}
