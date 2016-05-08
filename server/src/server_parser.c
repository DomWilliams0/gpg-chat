#include <argp.h>
#include <stdlib.h>
#include <string.h>
#include "server_parser.h"
#include "shared_utils.h"

static int parse_opt(int key, char *arg, struct argp_state *state)
{
	struct server_settings *settings = state->input;

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

				 settings->port = port;
			break;

		// paths
		case 'c':
		case 'k':;
				 int len = strlen(arg);
				 if (len > PATH_MAX)
				 {
					 fprintf(stderr, "Invalid path, max length is %d\n", PATH_MAX);
					 exit(ERROR_BAD_INPUT);
				 }

				 const char **path = key == 'c' ? &settings->cert_path : &settings->key_path;
				 *path = arg;
				 break;

		default:
			return ARGP_ERR_UNKNOWN;
	}
	
	return 0;
}

void parse_server_settings(int argc, char **argv, struct server_settings *out)
{
	struct argp_option options[] =
	{
		{"port", 'p', "PORT", 0, "Port to listen on (defaults to "DEFAULT_PORT_STR, 0},
		{"cert", 'c', "CERT_PATH", 0, "Path to TLS certificate", 0},
		{"key", 'k', "KEY_PATH", 0, "Path to TLS key", 0},
		{ 0, 0, 0, 0, 0, 0 }
	};

	char args_doc[] = "";
	char doc[] = "GPGChat server";

	struct argp argp = {options, parse_opt, args_doc, doc, NULL, NULL, NULL};

	// defaults
	out->port = DEFAULT_PORT;
	out->cert_path = NULL;
	out->key_path = NULL;

	argp_parse(&argp, argc, argv, 0, 0, out);

	// validation
	if (!out->cert_path)
	{
		fputs("Certificate path required\n", stderr);
		exit(ERROR_BAD_INPUT);
	}

	if (!out->key_path)
	{
		fputs("Key path required\n", stderr);
		exit(ERROR_BAD_INPUT);
	}
		
}
