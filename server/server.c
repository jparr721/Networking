#include <stdio.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
	// Variables
	int sock;
	struct sockaddr_in server;
	int connection;
	char buffer[1024];
	int rval;

	// Create Socket
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock < 0) {
		perror("Failed to create socket...");
		exit(1);
	}

	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(8080);

	// Bind	
	if (bind(sock, (struct sockaddr *)&server, sizeof(server))) {
		perror("Bind failed");
		exit(1);	
	}

	// Listen
	listen(sock, 5);

	// Accept
	do {
		connection = accept(sock, (struct sockaddr *) 0, 0);
		if ((connection = -1))
			perror("Accept Failed");
		else {
			memset(buffer, 0, sizeof(buffer));
			if  ((rval = recv(connection, buffer, sizeof(buffer), 0)) < 0) 
				perror("Reading stream message error");
			else if (rval == 0)
				printf("Ending connection \n");
			else
				printf("MSG: %s\n", buffer);
				
			printf("Got the message (rval = %d)\n", rval);
			close(connection);

		}

	} while(1);

	return 0;
}
