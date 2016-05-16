#include "shared_utils.h"
#include "server_actions.h"
#include "server_parser.h"
#include "socket.h"

int handle_server_action(struct server_settings *settings, SSL *connection, char *op)
{
	// sanity check
	if (strlen(op) != OP_LENGTH)
		error_ret("Invalid opcode length\n", ERROR_BAD_INPUT);

	if (strcmp(OP_CONNECT, op) == 0)
		return do_server_action_connect(settings, connection);

	if (strcmp(OP_MESSAGE, op) == 0)
		return do_server_action_message(settings, connection);

	error_ret("Unimplemented action\n", ERROR_BAD_OP);
}
