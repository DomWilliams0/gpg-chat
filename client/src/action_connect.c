#include <argp.h>
#include <memory.h>
#include <arpa/inet.h>
#include "client_actions.h"
#include "client_parser.h"
#include "shared_utils.h"
#include "socket.h"

int export_key(gpgme_data_t *out, char *key_id, GPG_CTX *gpg_ctx)
{
	// validation
	if (key_id == NULL)
		error_ret("Key to connect with is required\n", ERROR_BAD_INPUT);

	gpgme_key_t key;
	int ret;
	bool confirm;
	gpgme_error_t err;

	// find key to register
	ret = GPG_get_key(gpg_ctx, key_id, &key);
	if (is_failure(ret))
		error_ret("Could not corresponding key\n", ERROR_GPGME);

	printf("Going to connect with %s (%s <%s>)\n", 
			key->subkeys->keyid,
			key->uids->name,
			key->uids->email
			);

	// ensure key is valid
	if (!key->can_encrypt || key->revoked || key->disabled || key->expired)
		error_ret("This key cannot be used\n", ERROR_GPGME);

	// confirm
	confirm = request_confirmation("OK to proceed?");
	if (!confirm)
	{
		puts("Aborting");
		return ERROR_USER_ABORT;
	}

	// get key
	err = gpgme_data_new(out);
	if (gpg_err_code(err) != GPG_ERR_NO_ERROR)
		error_ret("Failed to allocate key buffer\n", ERROR_GPGME);

	ret = GPG_export(gpg_ctx, key, *out);
	if (is_failure(ret))
		error_ret("Failed to export key\n", ERROR_GPGME);

	return ERROR_NO_ERROR;
}

int send_to_server(gpgme_data_t key, char *host, unsigned short port, GPG_CTX *gpg_ctx)
{
	SSL_CTX *ssl_ctx;
	SSL *ssl;
	int sock;

	// connect to server
	init_ssl(&ssl_ctx, &ssl);
	sock = create_socket(host, port, ssl);
	if (sock < 0)
		return ERROR_SOCKET;

	// C -> S: REG|public key
	// opcode
	SSL_write(ssl, OP_CONNECT, OP_LENGTH);

	// public key
	uint64_t key_length_net;
	size_t key_length_var;
	char *key_buf;
	key_buf = gpgme_data_release_and_get_mem(key, &key_length_var);
	if (key_buf == NULL)
		error_ret("Failed to copy key data\n", ERROR_GPGME);

	// length|data
	key_length_net = htobe64(key_length_var);
	SSL_write(ssl, &key_length_net, sizeof key_length_net);
	SSL_write(ssl, key_buf, key_length_var);

	// TODO: receive challenge from server

	UNUSED(gpg_ctx);

	return ERROR_NO_ERROR;
}

int do_action_connect(struct client_settings *settings, GPG_CTX *gpg_ctx)
{
	gpgme_data_t key;
	int ret;

	// get key
	ret = export_key(&key, settings->key, gpg_ctx);
	if (is_failure(ret))
		return ret;

	// send to server
	ret = send_to_server(key, settings->host, settings->host_port, gpg_ctx);
	if (ret != ERROR_NO_ERROR)
		return ret;

	return ERROR_NO_ERROR;
}
