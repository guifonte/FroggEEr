#include "../../inc/controller/server.hpp"

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

Server::Server() {

}

int Server::init(unsigned int port){
  int socket_fd;
  struct sockaddr_in myself;

  socket_fd = socket(AF_INET, SOCK_STREAM, 0);

  myself.sin_family = AF_INET;
  myself.sin_port = htons(port);
  inet_aton("0.0.0.0", &(myself.sin_addr));

  if (::bind(socket_fd, (struct sockaddr*)&myself, sizeof(myself)) != 0) {
    //return 0;
  }
  return socket_fd;
}

void Server::accept_connections(int socket_fd, int *connection_fd){
  struct sockaddr_in client;
  socklen_t client_size = (socklen_t)sizeof(client);
  int conn_fd;
  int user_flag=0;

  listen(socket_fd, 2);

  int user_count=0;
  while(user_count!=MAX_CONEXOES) {
    conn_fd = accept(socket_fd, (struct sockaddr*)&client, &client_size);
    connection_fd[user_count] = conn_fd;
    user_count++;
    printf("Novo usuario chegou!\n");
  }
  
}

void Server::run(int *socket_fd, char *key, int *connection_fd){
  struct sockaddr_in client;
  socklen_t client_size = (socklen_t)sizeof(client);
  char input_buffer[50];
  while (1) {
    for (int i=0; i<MAX_CONEXOES; i++) {
      recv(connection_fd[i], input_buffer, 10, 0);
      key[i] = input_buffer[0];
    }
  }
  close(*socket_fd);
}