#include "../../inc/controller/server.hpp"

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

Server::Server() {

}

void Server::init(int *socket_fd, unsigned int port){
  struct sockaddr_in myself;

  *socket_fd = socket(AF_INET, SOCK_STREAM, 0);

  // myself.sin_family = AF_INET;
  // myself.sin_port = htons(port);
  // inet_aton("0.0.0.0", &(myself.sin_addr));


    myself.sin_family = AF_INET;         /* host byte order */
    myself.sin_port = htons(port);     /* short, network byte order */
    myself.sin_addr.s_addr = INADDR_ANY; /* auto-fill with my IP */
    //bzero(&(myself.sin_zero), 8);        /* zero the rest of the struct */

  if (::bind(*socket_fd, (struct sockaddr*)&myself, sizeof(myself)) != 0) {
    //return 0;
  }
}

void Server::accept_connections(int *socket_fd, int *connection_fd){
  struct sockaddr_in client;
  socklen_t client_size = (socklen_t)sizeof(client);
  int conn_fd;
  int user_flag=0;

  listen(*socket_fd, 2);

  int user_count=0;
  while(user_count!=MAX_CONEXOES) {
    conn_fd = accept(*socket_fd, (struct sockaddr*)&client, &client_size);
    fcntl(conn_fd, F_SETFL, O_NONBLOCK); /* Change the socket into non-blocking state	*/
    connection_fd[user_count] = conn_fd;
    user_count++;
    printf("Novo usuario chegou!\n");
  }  
}

void Server::run(int *socket_fd, char *key, int *connection_fd){
  struct sockaddr_in client;
  socklen_t client_size = (socklen_t)sizeof(client);
  char input_buffer[2];
  int msglen=0;

  while (1) {
    for (int i=0; i<MAX_CONEXOES; i++) {
      msglen=recv(connection_fd[i], input_buffer, 2, 0);
      printf("Msglen: %d User: %d\n", msglen, i);
      if (msglen>0) {
        key[i] = input_buffer[0];
        printf("Received char %c from user %d\n", key[i], i);
      }
    }
  }
  close(*socket_fd);
}