#include <argp.h>
#include <stdlib.h>
#include <string.h>
#include "client_parser.h"
#include "client_actions.h"
#include "shared_utils.h"

static bool parse_client_action(char *input, enum client_action *out)
{
	if (strcmp("message", input) == 0)
	{
		*out = MESSAGE;
		return true;
	}
	else if (strcmp("connect", input) == 0)
	{
		*out = CONNECT;
		return true;
	}

	return false;
}

static int parse_opt(int key, char *arg, struct argp_state *state)
{
	struct client_settings *settings = state->input;
	int port;
	bool succ;

	switch(key)
	{
		// action
		case 'a':
			succ = parse_client_action(arg, &settings->action);
			if (!succ)
				argp_error(state, "Invalid action\n");
			break;

		// port
		case 'p':
			port = strtol(arg, NULL, 10);
			if (port < 1 || port > 65535)
				argp_error(state, "Invalid port\n");

			settings->host_port = port;
			break;

		// host
		case 'h':
			settings->host = arg;
			break;

		// sign
		case 's':
			settings->sign_message = true;
			break;

		// key
		case 'k':
			settings->key = arg;
			break;

		// recipients
		case ARGP_KEY_ARG:
			settings->recipients = &state->argv[state->next];
			state->next = state->argc;
			break;

		default:
			return ARGP_ERR_UNKNOWN;
	}

	return 0;
}

void parse_client_settings(int argc, char **argv, struct client_settings *out)
{
	struct argp_option options[] =
	{
		{"action", 'a', "ACTION", 0, "One of \"connect\" or \"message\" (default)", 0},
		{"host", 'h', "HOST", 0, "Remote host to connect to", 0},
		{"port", 'p', "PORT", 0, "Remove host port (defaults to "DEFAULT_PORT_STR, 0},

		// messaging
		{ 0, 0, 0, 0, "Messaging specific parameters:", 1},
		{"sign", 's', 0, 0, "Sign the message", 1},
	
		// connecting
		{ 0, 0, 0, 0, "Registering specific parameters:", 2},
		{"key", 'k', "KEY", 0, "Your public key", 2},

		{ 0, 0, 0, 0, 0, 0 }
	};

	char args_doc[] = "[RECIPIENT...]";
	char doc[] = "GPGChat client";

	struct argp argp = {options, parse_opt, args_doc, doc, NULL, NULL, NULL};

	// defaults
	bzero((char *) out, sizeof(struct client_settings));
	out->action = MESSAGE;
	out->host_port = DEFAULT_PORT;
	out->sign_message = true;

	argp_parse(&argp, argc, argv, 0, 0, out);
	// TODO load config and don't override options
}
