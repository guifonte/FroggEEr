
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "oo_model.hpp"

int main() {
  int socket_fd;
  struct sockaddr_in target;
  char character[2];
  char serverIp[16];
  
  printf("IP do servidor:\n");
  scanf("%s", serverIp);	
  socket_fd = socket(AF_INET, SOCK_STREAM, 0);
  printf("Socket criado\n");

  target.sin_family = AF_INET;
  target.sin_port = htons(3001);
  inet_aton(serverIp, &(target.sin_addr));
  printf("Tentando conectar\n");
  if (connect(socket_fd, (struct sockaddr*)&target, sizeof(target)) != 0) {
    printf("Problemas na conexao\n");
    return 0;
  }
  printf("Conectei ao servidor\n");

  int winX = 15;
  int winY = 51;

  showStartFrog();

  char c,c2;//char clicado no teclado
  char cPrev = 0;//char anterior clicado pelo teclado
  char key = '0';

  RelevantData *rd = new RelevantData(*player, *l);
  Teclado *teclado = new Teclado();
  teclado->init();

  char *bufferchar = new char[buffer.length()+1];
  recv(socket_fd, bufferchar, strlen(bufferchar), 0);
  rd->unserialize(bufferchar);

  // TODO: fix level
  Tela *tela = new Tela(rd->player, rd->l, &level ,winX, winY, winX, winY);
  tela->init();

  while (1) {

    recv(socket_fd, bufferchar, strlen(bufferchar), 0);
    rd->unserialize(bufferchar);
    
    // Atualiza tela
    tela->update();

    // Lê o teclado
    c = teclado->getchar();
    if (c != cPrev){ //evita que o usuário deixe o botão pressionado para andar mais rápido
      if (c=='w') {
        send(socket_fd, c, 2, 0);
      }
      if (c=='a') {
        send(socket_fd, c, 2, 0);
      }
      if (c=='s') {
        send(socket_fd, c, 2, 0);
      }
      if (c=='d') {
        send(socket_fd, c, 2, 0);
      }
      if (c=='q') {
        break;
      }
    }
    cPrev = c;

    std::this_thread::sleep_for (std::chrono::milliseconds(100));

  }
  
  tela->stop();
  teclado->stop();
  close(socket_fd);
  return 0;
}


