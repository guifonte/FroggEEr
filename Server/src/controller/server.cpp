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
  int new_conn_fd;
  int user_id=-1;
  struct sockaddr_in myself;
  struct sockaddr_in client;
  socklen_t client_size = (socklen_t)sizeof(client);

  for (int i=0; i<MAX_CONEXOES; i++) {
    conexao_usada[i] = 0;
  }

  socket_fd = socket(AF_INET, SOCK_STREAM, 0);

  myself.sin_family = AF_INET;
  myself.sin_port = htons(port);
  inet_aton("0.0.0.0", &(myself.sin_addr));

  if (::bind(socket_fd, (struct sockaddr*)&myself, sizeof(myself)) != 0) {
    //return 0;
  }

  listen(socket_fd, 2);
  
  int i=0;
  while(i<MAX_CONEXOES) {
    new_conn_fd = accept(socket_fd, (struct sockaddr*)&client, &client_size);
    for (i=0; i<MAX_CONEXOES; i++) {
      if (conexao_usada[i] == 0) {
        conexao_usada[i] = 1;
        connection_fd[i] = new_conn_fd;
        user_id=i;
      }
    }
    if (user_id != -1) {
      printf("Novo usuario chegou! ID=%d\n", user_id);
      user_id=-1;
    } else {
      printf("Maximo de usuarios atingido!\n");
    }
  }


  return socket_fd;
}

void Server::run(char *key){
  int *connection_fd;
  struct sockaddr_in client;
  socklen_t client_size = (socklen_t)sizeof(client);

  char input_buffer[50];

  
  //*connection_fd = accept(*socket_fd, (struct sockaddr*)&client, &client_size);

  /* Identificando cliente 
  char ip_client[INET_ADDRSTRLEN];
  inet_ntop( AF_INET, &(client.sin_addr), ip_client, INET_ADDRSTRLEN );
  printf("IP que enviou: %s\n", ip_client);*/
  while (1) {
    for (int user_iterator=0; user_iterator<MAX_CONEXOES; user_iterator++) {
      if (conexao_usada[user_iterator] == 1) {
        recv(*connection_fd, input_buffer, 10, 0);
        *key = input_buffer[0];
      }
    }
    //printf("%s\n", input_buffer);

    /* Respondendo */
    /*if (send(*connection_fd, "PONG", 5, 0) < 0) {
      //printf("Erro ao enviar mensagem de retorno\n");
    } else {
     // printf("Sucesso para enviar mensagem de retorno\n");
    }*/
  }

  // /close(*socket_fd);
}