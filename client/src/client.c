#include <stdio.h>
#include <unistd.h>
#include <openssl/err.h>
#include <openssl/ssl.h>
#include "shared_utils.h"
#include "client_actions.h"
#include "client_parser.h"
#include "gpg.h"

int main(int argc, char **argv) 
{
	int ret;

	// load arguments
	struct client_settings settings;
	ret = parse_client_settings(argc, argv, &settings);
	if (ret != ERROR_NO_ERROR)
	{
		print_usage();
		return ret;
	}

	// init gpg
	GPG_CTX *gpg_ctx;
	ret = GPG_CTX_new(&gpg_ctx);
	if (is_failure(ret))
		goto GENERAL_CLEAN_UP; // *retch*

	// handle action
	ret = handle_action(&settings, gpg_ctx);

	if (ret == ERROR_BAD_INPUT)
		print_usage();

	else if (ret == ERROR_USER_ABORT)
		ret = ERROR_NO_ERROR;

	// clean up
	GPG_CTX_free(gpg_ctx);

GENERAL_CLEAN_UP:
	ERR_free_strings();
	EVP_cleanup();

	return ret;
}
