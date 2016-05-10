#include <argp.h>
#include <memory.h>
#include "client_actions.h"
#include "client_parser.h"
#include "shared_utils.h"
#include "socket.h"

bool export_key(gpgme_data_t *out, char *key_id, GPG_CTX *gpg_ctx)
{
	// validation
	if (key_id == NULL)
		error_argp("Key to connect with is required\n");

	gpgme_key_t key;
	bool succ;
	gpgme_error_t err;

	// find key to register
	succ = GPG_get_key(gpg_ctx, key_id, &key);
	if (!succ)
		error("Could not corresponding key");

	printf("Going to connect with %s (%s <%s>)\n", 
			key->subkeys->keyid,
			key->uids->name,
			key->uids->email
			);

	// ensure key is valid
	if (!key->can_encrypt || key->revoked || key->disabled || key->expired)
		error("This key cannot be used");

	// confirm
	succ = request_confirmation("OK to proceed?");
	if (!succ)
	{
		puts("Aborting");
		return false;
	}

	// get key
	err = gpgme_data_new(out);
	if (gpg_err_code(err) != GPG_ERR_NO_ERROR)
		error("Failed to allocate key buffer");

	succ = GPG_export(gpg_ctx, key, *out);
	if (!succ)
		error("Failed to export key");

	return true;
}

bool send_to_server(gpgme_data_t key, char *host, unsigned short port, GPG_CTX *gpg_ctx)
{
	SSL_CTX *ssl_ctx;
	SSL *ssl;
	int sock;

	// connect to server
	init_ssl(&ssl_ctx, &ssl);
	sock = create_socket(host, port, ssl);
	UNUSED(sock);

	// C -> S: REG|public key
	// opcode
	SSL_write(ssl, OP_CONNECT, OP_LENGTH);

	// public key
	char buf[PACKET_SIZE];
	ssize_t n;
	while (true)
	{
		n = gpgme_data_read(key, buf, PACKET_SIZE);
		SSL_write(ssl, buf, n);

		// eof
		if (n < PACKET_SIZE)
			break;
	}

	// TODO: receive challenge from server

	UNUSED(gpg_ctx);

	return true;
}

void do_action_connect(struct client_settings *settings, GPG_CTX *gpg_ctx)
{
	gpgme_data_t key;
	bool succ;

	// get key
	succ = export_key(&key, settings->key, gpg_ctx);
	if (!succ)
		return;

	// send to server
	succ = send_to_server(key, settings->host, settings->host_port, gpg_ctx);
	if (!succ)
		return;



	return;

}
