#include <argp.h>
#include "client_actions.h"
#include "client_parser.h"
#include "shared_utils.h"

void do_action_register(struct client_settings *settings, GPG_CTX *gpg_ctx)
{
	// validation
	if (settings->key == NULL)
		error_argp("Key to register is required\n");

	bool succ;
	gpgme_key_t key;

	// find key to register
	succ = GPG_get_key(gpg_ctx, settings->key, &key);
	if (!succ)
		error("Could not corresponding key");

	printf("Registering key for %s\n", key->uids->name);
}
