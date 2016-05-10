#include <argp.h>
#include "client_actions.h"
#include "client_parser.h"
#include "shared_utils.h"

int do_action_message(struct client_settings *settings, GPG_CTX *gpg_ctx)
{
	// validation
	if (settings->recipients == NULL)
		error_ret("Recipients required\n", ERROR_BAD_INPUT);

	UNUSED(gpg_ctx);
	puts("TODO: Messaging...");

	return ERROR_NO_ERROR;
}
