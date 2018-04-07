#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>

#define PORT "3490" // The port the bot machines will connect to

#define BACKLOG 100 // The max number of machines (100 should be plenty)

void sigchld_handler(int s) {
	// waitpid() could overrite errno, we need to save it
	int saved_errno = errno;

	while(waitpid(-1, NULL, WNOHANG) > 0);

	errno = saved_errno;
}

void *get_in_addr(struct sockaddr *saddr) {
	if (saddr->sa_family == AF_INET) {
		return &(((struct sockaddr_in*)saddr)->sin_addr);
	}
	return &(((struct sockaddr_in6*)saddr)->sin6_addr);
}

int server_loop() {
	int sockfd, newfd;
	struct addrinfo hints, *servinfo, *p;
	struct sockaddr_storage bot_addr;
	socklen_t sin_size;
	struct sigaction sa;
	int yes = 1;
	char s[INET_ADDRSTRLEN];
	int rv;
	
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	if ((rv = getaddrinfo(NULL, PORT, &hints, &servinfo)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		return 1;
	}

	for (p = servinfo; p != NULL; p = p->ai_next) {
		if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
			perror("server: socket");
			continue;
		}

		if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
			perror("setsockopt");
			exit(1);
		}

		if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
			close(sockfd);
			perror("server: build");
			continue;
		}
		break;
	}

	freeaddrinfo(servinfo);

	if (p == NULL) {
		fprintf(stderr, "server: failed to bind\n");
		exit(1);
	}

	if (listen(sockfd, BACKLOG) == -1) {
		perror("listen");
		exit(1);
	}

	sa.sa_handler = sigchld_handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	if (sigaction(SIGCHLD, &sa, NULL) == -1) {
		perror("sigaction");
		exit(1);
	}

	printf("server: waiting for connections...\n");

	while(1) {
		sin_size = sizeof bot_addr;
		newfd = accept(sockfd, (struct sockaddr *)&bot_addr, &sin_size);
		if (newfd == -1) {
			perror("accept");
			continue;
		}	

		inet_ntop(bot_addr.ss_family, get_in_addr((struct sockaddr *)&bot_addr), s, sizeof s);
		printf("server: got connection from %s\n", s);

		if (!fork()) {
			close(sockfd);
			if (send(newfd, "Connection live...", 18, 0) == -1)
				perror("send");
			close(newfd);
			exit(0);
		}
		close(newfd);
	}
	return 0;
}
