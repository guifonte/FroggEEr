/*
 * Arquivo: 	model_mainloop.cpp
 * Autores:	  Guilherme Nishino Fontebasso
 *		        Cynthia Baran
 * Descrição:	Implementação MVC de uma implementação do
 *		        jogo Frogger utilizando ALSA para os sons
 *		        e ncurses para a visualização
 * Comandos:	WASD para mover o asterisco (player)
 * 		        Q para sair
*/
#include <iostream>
#include <chrono>
#include <thread>
#include <vector>

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <string>
#include <cstring>

//model
#include "../inc/model/lane.hpp"
#include "../inc/model/player.hpp"
//view
#include "../inc/view/audioPlayer.hpp"
#include "../inc/view/tela.hpp"
#include "../inc/view/teclado.hpp"
//control
#include "../inc/controller/server.hpp"
#include "../inc/controller/fisica.hpp"
#include "../inc/controller/serialize.hpp"



using namespace std::chrono;

uint64_t get_now_ms() {
  return duration_cast<milliseconds>(steady_clock::now().time_since_epoch()).count();
}

int main ()
{
  srand(time(NULL));

  int winX = 15;
  int winY = 51;

  freopen("/dev/null", "w", stderr);

  //1 because of the botton besel and 2 because of the start safe zone
  int laneStartX = winX-3; 
  int laneY = winY-2; //2 because of the left and right besel
  //3 for the upper header, 1 for the botton besel and 4 for the safe zones
  //but there are winX+1 lines, because of that, -7
  //int maxNumLanes = winX-7;
  int maxNumLanes = 8; 
  int numOfLanes;
  Lane lanes[8];
  std::vector<Lane*> *vecLanes;

  int level = 1;
  int nextLevel = 1 ; //boolean que indica se é necessário mudar de nível e gerar novas lanes

  int touched = 0;//indica se player tocou em algo

  char c,c2;//char clicado no teclado
  char cPrev = 0;//char anterior clicado pelo teclado

  Player *player = new Player(laneStartX+2, laneY/2);
  ListaDeLanes *l = new ListaDeLanes();

  RelevantData *rd;
  string buffer;

  Fisica *f = new Fisica(l,player);

  Teclado *teclado = new Teclado();
  teclado->init();

  Server *server = new Server();
  int socket_fd = server->init(3001);
  int connection_fd;
  char key = '0';
  std::thread serverthread(Server::run, &socket_fd, &key, &connection_fd);

  //Tela *tela = new Tela(player, l, &level ,winX, winY, winX, winY);
  //tela->showStartFrog();
  //tela->init();

  //Audio::SoundManager *soundManager = new Audio::SoundManager("res/");

  uint64_t t0;
  uint64_t t1;
  uint64_t deltaT;
  uint64_t T;

  T = get_now_ms();
  t1 = T;

  while (1) {
    // Atualiza timers
    t0 = t1;
    t1 = get_now_ms();
    deltaT = t1-t0;

    //verifica se muda de nível.
    //Se sim, apaga as lanes anteriores e cria novas com o novo nível
    //Move o player para posição inicial

    if(nextLevel == 1) {
      nextLevel = 0;
      l->clearLanes();
      //tela->clearLaneArea();
      l->createLanes(maxNumLanes,laneY,laneStartX, level);
      player->resetPos();
    }


    // Atualiza modelo
    f->update(deltaT);

    //printf("updated\n");
    touched = f->hasTouched();

    // Atualiza tela
    //tela->update();

    // Lê o teclado
    c = teclado->getchar();
    if (c != cPrev){ //evita que o usuário deixe o botão pressionado para andar mais rápido
      if (c=='w') {
        if(player->getX() > 3)
          player->update(player->getX()-1,player->getY());
      }
      if (c=='a') {
        if(player->getY() > 1)
          player->update(player->getX(),player->getY()-1);
      }
      if (c=='s') {
        if(player->getX() < laneStartX+2)
          player->update(player->getX()+1,player->getY());
      }
      if (c=='d') {
        if(player->getY() < laneY+2)
          player->update(player->getX(),player->getY()+1);
      }
      if (c=='q') {
        break;
      }
    }
    cPrev = c;

    c2 = key;
    if (c2=='w') {
      if(player->getX() > 3)
        player->update(player->getX()-1,player->getY());
    }
    if (c2=='a') {
      if(player->getY() > 1)
        player->update(player->getX(),player->getY()-1);
    }
    if (c2=='s') {
      if(player->getX() < laneStartX+2)
        player->update(player->getX()+1,player->getY());
    }
    if (c2=='d') {
      if(player->getY() < laneY+2)
        player->update(player->getX(),player->getY()+1);
    }
    if (c2=='q') {
      break;
    }
    key = '0';

    // Verifica se tocou em algum bloco

    if(touched == 1){
      //soundManager->playKillSound(t0);
      player->resetPos();
      touched = 0; 
    }
    
    // Verifica se atravessou as lanes
    if(player->getX() <= (laneStartX-(l->getNumberOfLanes()))) {
      level++;
      nextLevel++;
      //soundManager->playLevelUpSound(t0);
    }
    numOfLanes = l->getNumberOfLanes();
    vecLanes = l->getLanes();
    for(int i = 0; i < numOfLanes; i++){
      lanes[i] = *(*vecLanes)[i];
    }
    printf("Pre rd\n");
    rd = new RelevantData(*player, lanes, level,numOfLanes);
    printf("Pre serialize\n");
    rd->serialize(buffer);
    char *bufferchar = new char[buffer.length()+1];
    printf("%lu\n",buffer.length());
    strcpy(bufferchar,buffer.c_str());
    printf("Pre send\n");
    send(connection_fd, bufferchar, strlen(bufferchar), 0);

    // Condicao de parada
    if ( (t1-T) > 1000000 ) break;

    std::this_thread::sleep_for (std::chrono::milliseconds(100));

  }
  
  //tela->stop();
  teclado->stop();
  //tela->showSadFrog();
  return 0;
}
