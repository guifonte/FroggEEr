
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>


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
  system ("/bin/stty raw");
  character[1] = '\0';
  /* Agora, meu socket funciona como um descritor de arquivo usual */
  while(character[0] != 'q'){
    scanf(" %c",&character[0]);
    getchar();
    printf("ok");
    if(character[0] != ' '){
      send(socket_fd, character, 2, 0);
      printf("Enviei mensagem com o caractere %c\n", character[0]);
    }
  }

  /* Recebendo resposta */
  char reply[10];
  recv(socket_fd, reply, 10, 0);
  printf("Resposta:\n%s\n", reply);
  close(socket_fd);
  system ("/bin/stty cooked");
  return 0;
}


