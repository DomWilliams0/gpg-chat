#include <argp.h>
#include <stdlib.h>
#include <string.h>
#include "client_parser.h"
#include "shared_utils.h"

static int parse_opt(int key, char *arg, struct argp_state *state)
{
	struct client_settings *settings = state->input;

	switch(key)
	{
		// port
		case 'p':;
 				 int port = strtol(arg, NULL, 10);
 				 if (port < 1 || port > 65535)
				 {
					 fputs("Invalid port\n", stderr);
					 exit(ERROR_BAD_INPUT);
				 }

				 settings->host_port = port;
			break;

		// host
		case 'h':;
				 settings->host = arg;
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
		{"host", 'h', "HOST", 0, "Remote host to connect to", 0},
		{"port", 'p', "PORT", 0, "Remove host port", 0},
		{ 0, 0, 0, 0, 0, 0 }
	};

	char args_doc[] = "";
	char doc[] = "GPGChat client";

	struct argp argp = {options, parse_opt, args_doc, doc, NULL, NULL, NULL};

	// defaults
	out->host_port = 11500;
	out->host = NULL;

	argp_parse(&argp, argc, argv, 0, 0, out);

	// validation
	if (!out->host)
	{
		fputs("Host required\n", stderr);
		exit(ERROR_BAD_INPUT);
	}
}
