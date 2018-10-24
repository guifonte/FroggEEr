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

void Server::run(int *socket_fd, char *key, int *connection_fd){
  struct sockaddr_in client;
  socklen_t client_size = (socklen_t)sizeof(client);
  char input_buffer[50];

  listen(*socket_fd, 2);
  *connection_fd = accept(*socket_fd, (struct sockaddr*)&client, &client_size);

  /* Identificando cliente 
  char ip_client[INET_ADDRSTRLEN];
  inet_ntop( AF_INET, &(client.sin_addr), ip_client, INET_ADDRSTRLEN );
  printf("IP que enviou: %s\n", ip_client);*/
  while (1) {
    recv(*connection_fd, input_buffer, 10, 0);
    *key = input_buffer[0];
    //printf("%s\n", input_buffer);

    /* Respondendo */
    /*if (send(*connection_fd, "PONG", 5, 0) < 0) {
      //printf("Erro ao enviar mensagem de retorno\n");
    } else {
     // printf("Sucesso para enviar mensagem de retorno\n");
    }*/
  }

  close(*socket_fd);
}