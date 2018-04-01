#include<iostream>
#include<vector>
#include<stdlib.h>
#include<unistd.h>
#include<errno.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netdb.h>
#include<arpa/inet.h>
#include<sys/wait.h>
#include<signal.h>
#include "../includes/server.hpp"

#define PORT "3490" // The port the bot machines will connect to

#define BACKLOG 100 // The max number of machines (100 should be plenty)

void sigchld_handler(int s) {
	// waitpid() could overrite errno, we need to save it
	int saved_errno = errno;

	while(waitpid(-1, NULL, WNOHANG) > 0);

	errno = saved_errno;
}

// Determine IPv4 or IPv6 (should be IPv4)
void* Server::get_in_addr(struct sockaddr *socket_address) {
	// If address is v4 then return in that context otherwise do v6
	if (socket_address->sa_family == AF_INET) {
		return &(((struct sockaddr_in*)socket_address)->sin_addr);
	}
	return &(((struct sockaddr_in6*)socket_address)->sin6_addr);
}

int Server::server_loop() {
	// Listen on the socket file descriptor, new connections on new_fd
	int socket_fd, new_fd;
	struct addrinfo hints, *servinfo, *p; // To avoid memset on global vars
	struct sockaddr_storage bot_addr; // The bot address
	socklen_t sin_size;
	struct sigaction sa;
	int yes = 1;
	char s[INET6_ADDRSTRLEN];
	int rv;
	const void *test_buffer = "Server transmitting";

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE; // Use command system IP

	if ((rv = getaddrinfo(NULL, PORT, &hints, &servinfo)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		return 1;
	}

	/**
	 * This loops through the converted address
	 * which is an ip address, and then we 
	 * bind to the first one we can
	 */
	for (p = servinfo; p != NULL; p = p->ai_next) { // Iterating through the linked list
		if ((socket_fd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
			perror("server: socket");
			continue;
		}

		if (setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
			perror("setsockopt");
			exit(1);
		}

		if (bind(socket_fd, p->ai_addr, p->ai_addrlen) == -1) {
			close(socket_fd);
			perror("server: bind");
			continue;
		}
		break;
	}

	freeaddrinfo(servinfo); // Finished with this structure

	if (p == NULL) {
		fprintf(stderr, "server: failed to bind\n");
		exit(1);
	}

	if (listen(socket_fd, BACKLOG) == -1) {
		perror("listen");
		exit(1);
	}

	sa.sa_handler = sigchld_handler; // reap dead processes
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	if (sigaction(SIGCHLD, &sa, NULL) == -1) {
		perror("sigaction");
		exit(1);
	}

	printf("server: waiting for connections...\n");
	while(1) {
		sin_size = sizeof bot_addr;
		new_fd = accept(socket_fd, (struct sockaddr *)&bot_addr, &sin_size);
		if (new_fd == -1) {
			perror("Error accepting connection");
			continue;
		}

		inet_ntop(bot_addr.ss_family, get_in_addr((struct sockaddr *)&bot_addr), s, sizeof s);
		printf("received connection from: %s\n", s);
		// TODO -- Write this to the slave file for later use

		if (!fork()) {
			close(socket_fd);
			if (send(new_fd, test_buffer, sizeof(test_buffer), 0) == -1)
				perror("Send failed");
			close(new_fd);
			exit(0);	
		}
		
		// TODO -- CnC functionality
		close(new_fd);
	}
	return 0;
}
