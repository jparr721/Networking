/**
 * This code uses openssl to 
 * verify the certificate of
 * a client requesting server 
 * access. The intent of this
 * is to allow for only verified 
 * bots to connect to my CNC instance
 */
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <netdb.h>
#include <unistd.h>
#ifdef __VMS
#include <socket.h>
#include <inet.h>
#include <in.h>
#else
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#endif
#include <openssl/crypto.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

#define RSA_SERVER_CERT	"server.crt"
#define RSA_SERVER_KEY 	"server.key"

#define RSA_SERVER_CA_CERT 	"server_ca.cert"
#define RSA_SERVER_CA_PATH	"sys$common:[syshlp.titan.ssl]"

#define ON 1
#define OFF 0

#define RETURN_NULL(x) if ((x)  == NULL) exit(1)
#define RETURN_ERR(err, s) if ((err) == -1) { perror(s); exit(1); }
#define RETURN_SSL(err) if ((err) == -1) { ERR_print_errors_fp(stderr); exit(1); }

int check_certificate(short int port) {
	int err;
	int verify_client = OFF; // Set to ON to verify client certificate
	size_t client_len;
	char* str;
	char bug[4096];
	SSL_CTX* ctx;
	SSL* ssl;
	const SSL_METHOD* meth;
	X509 *client_cert = NULL;
	short int s_port = port;

	/** Load encryption & hashing algorithms **/
	SSL_library_init();
	/** Load error strings for SSL and CRYPTO **/
	SSL_load_error_strings();
	/** Create SSL_METHOD structure **/
	meth = SSLv23_method();
	/* Create SSL_CTX structure **/
	ctx = SSL_CTX_new(meth);

	if (!ctx) {
		ERR_print_errors_fp(stderr);
		exit(1);
	}

	/** Load server certificate into SSL_CTX structure **/
	if (SSL_CTX_use_certificate_file(ctx, RSA_SERVER_CERT, SSL_FILETYPE_PEM) <= 0) {
		ERR_print_errors_fp(stderr);
		exit(1);
	}

	/** Load the private-key to the server certificate **/
	if (SSL_CTX_use_PrivateKey_file(ctx, RSA_SERVER_KEY, SSL_FILETYPE_PEM) <= 0) {
		ERR_print_errors_fp(stderr);
		exit(1);
	}

	/** Check if the server certificate and private-key match **/
	if (!SSL_CTX_check_private_key(ctx)) {
		fprintf(stderr, "Private key does not math the certificate public key\n");
		exit(1);
	}

	if (verify_client == ON) {
		/** Load the RSA CA certificate into the SSL_CTX structure **/
		if (!SSL_CTX_load_verify_locations(ctx, RSA_SERVER_CA_CERT, NULL)) {
			ERR_print_errors_fp(stderr);
			exit(1);
		}
		/** Set to require peer (client) certificate verification **/
		SSL_CTX_set_verify(ctx, SSL_VERIFY_PEER, NULL);

		/** Set verification depth to 1 **/
		SSL_CTX_set_verify_depth(ctx, 1);
	}
	return 0;
}
