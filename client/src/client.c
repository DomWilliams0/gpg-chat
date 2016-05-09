#include <stdio.h>
#include <unistd.h>
#include <openssl/err.h>
#include <openssl/ssl.h>
#include "shared_utils.h"
#include "client_parser.h"
#include "gpg.h"

int main(int argc, char **argv) 
{
	// load arguments
	struct client_settings settings;
	parse_client_settings(argc, argv, &settings);

	// init gpg
	GPG_CTX *gpg_ctx;
	gpg_ctx = GPG_CTX_new();

	// handle action
	handle_action(&settings, gpg_ctx);

	// clean up
	GPG_CTX_free(gpg_ctx);
	ERR_free_strings();
	EVP_cleanup();

	return 0;
}
