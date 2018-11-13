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

int* Server::accept_connections(int socket_fd, int *connection_fd){
  int conexao_usada[MAX_CONEXOES];
  struct sockaddr_in client;
  socklen_t client_size = (socklen_t)sizeof(client);
  int conn_fd;
  int user_id;
  int new_connection_fd;

  listen(socket_fd, 2);

  for (int i=0; i<MAX_CONEXOES; i++) {
    conexao_usada[i] = 0;
  }

  while(conexao_usada[MAX_CONEXOES-1]!=1) {
    conn_fd = accept(socket_fd, (struct sockaddr*)&client, &client_size);
    int i;
    for (i=0; i<MAX_CONEXOES; i++) {
      if (conexao_usada[i] == 0) {
        conexao_usada[i] = 1;
        connection_fd[i] = conn_fd;
      }
    }
    if (user_id > 0) {
      printf("Novo usuario chegou!\n");
    } else {
      printf("Sem tentativa de conexão!\n");
    }
  }
  return connection_fd;
}

void Server::run(int *socket_fd, char *key, int *connection_fd){
  struct sockaddr_in client;
  socklen_t client_size = (socklen_t)sizeof(client);
  char input_buffer[50];

  // listen(*socket_fd, 2);
  // *connection_fd = accept(*socket_fd, (struct sockaddr*)&client, &client_size);

  /* Identificando cliente 
  char ip_client[INET_ADDRSTRLEN];
  inet_ntop( AF_INET, &(client.sin_addr), ip_client, INET_ADDRSTRLEN );
  printf("IP que enviou: %s\n", ip_client);*/
  while (1) {
    for (int i=0; i<MAX_CONEXOES; i++) {
      recv(*(connection_fd+i), input_buffer, 10, 0);
      *(key+i) = input_buffer[0];
    }
    
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