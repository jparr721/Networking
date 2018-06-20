#include <stdio.h>
#include <signal.h> // SA_RESTART
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>

char* file_exists(char* filename) {
  char* data_buffer = 0;
  long length;
  FILE *f = fopen(filename, "rb");
  if (f) {
    fseek(f, 0, SEEK_END);
    length = ftell (f);
    fseek(f, 0, SEEK_SET);
    data_buffer = malloc(length);
    if (data_buffer) {
      fread(data_buffer, 1, length, f);
    }
    fclose(f);
  }

  if (data_buffer) {
    return data_buffer;
  }
  return "Unable to open the file";
}

int main(void) {
  char client_ip[INET6_ADDRSTRLEN];
  // create a new ipv4 socket
  int sockfd = socket(AF_INET, SOCK_STREAM, 0);

  struct sockaddr_in server, client;
  server.sin_family = AF_INET;
  server.sin_port = htons(8080);
  server.sin_addr.s_addr=INADDR_ANY;

  bind(sockfd, (struct sockaddr*) &server, sizeof(server));
  listen(sockfd, 10);
  printf("Server now accepting connections...");

  while (1) {
    int clientsock = accept(sockfd, (struct sockaddr*) &client, sizeof(client));
    inet_ntop(client.ss_family, get_in_addr((struct sockaddr*) &client), client_ip, sizeof client_ip);
    printf("New connection from %s\n", client_ip);

    char file_name[100];
    recv(clientsock, file_name, 100, 0);
    char* message = "Checking on that for you...";
    send(clientsock, message, sizeof(message), 0);

    char* file_data = file_exists(file_name);
    send(clientsock, file_data, sizeof(file_data), 0);

    printf("Data sent to client successfully");
    close(clientsock);
  }
}
