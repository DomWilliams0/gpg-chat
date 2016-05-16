#include <argp.h>
#include <stdlib.h>
#include <string.h>
#include "server_parser.h"
#include "shared_utils.h"

static int parse_opt(int key, char *arg, struct argp_state *state)
{
	struct server_settings *settings = state->input;
	int port, len;

	// aborted
	if (!settings->valid)
		return 0;

	switch(key)
	{
		// port
		case 'p':
			port = parse_port(arg);
			if (is_failure(port))
				parse_error("Invalid port\n");

			settings->port = port;
			break;

		// paths
		case 'c':
		case 'k':
			len = strlen(arg);
			if (len > PATH_MAX)
			{
				char err_buf[64];
				sprintf(err_buf, "Invalid path, max length is %d\n", PATH_MAX);
				parse_error(err_buf);
				break;
			}

			char **path = key == 'c' ? &settings->cert_path : &settings->key_path;
			*path = arg;
			break;

		default:
			return ARGP_ERR_UNKNOWN;
	}
	
	return 0;
}

int parse_server_settings(int argc, char **argv, struct server_settings *out)
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
	bzero((char *) out, sizeof(struct server_settings));
	out->port = DEFAULT_PORT;
	out->valid = true;

	argp_parse(&argp, argc, argv, 0, 0, out);

	// validation
	if (!out->cert_path)
		error_ret("Certificate path required\n", ERROR_BAD_INPUT);

	if (!out->key_path)
		error_ret("Key path required\n", ERROR_BAD_INPUT);

	return ERROR_NO_ERROR;
}
