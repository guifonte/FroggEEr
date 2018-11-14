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

int main (){
  srand(time(NULL));
  int levelUpCount[MAX_CONEXOES];
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
  //Lane lanes[8];
  std::vector<Lane*> *vecLanes;

  int level = 1;
  int nextLevel = 1 ; //boolean que indica se é necessário mudar de nível e gerar novas lanes

  int touched = 0;//indica se player tocou em algo

  char c,c2;//char clicado no teclado
  char cPrev = '0';//char anterior clicado pelo teclado

  ListaDePlayers *lp = new ListaDePlayers();
  ListaDeLanes *l = new ListaDeLanes();

  for (int i=0; i<MAX_CONEXOES; i++) {
    // fix start position
    Player *player = new Player(laneStartX+2, laneY/2);
    lp->addPlayer(player);
  }

  for (int i=0; i<MAX_CONEXOES; i++) {
    levelUpCount[i]=0;
  }

  RelevantData *rd;
  string bufferStr;
  std::vector<char> buffer;

  Fisica *f = new Fisica(l,lp);

  Teclado *teclado = new Teclado();
  teclado->init();

  Server *server = new Server();

  int connection_fd[MAX_CONEXOES];
  for (int i=0; i<MAX_CONEXOES; i++) {
    connection_fd[i]=0;
  }

  char key[MAX_CONEXOES];
  for (int i=0; i<MAX_CONEXOES; i++) {
   key[i] = '0';
  }

  int socket_fd; 

  server->init(&socket_fd, 3001);
  server->accept_connections(&socket_fd, connection_fd);

  std::thread serverthread(Server::run, &socket_fd, key, connection_fd);

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
  std::vector<Player *> *players = lp->getPlayers();

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
      for(int i = 0; i < (*players).size(); i++) {
        (*players)[i]->resetPos();
      }
    }

    // Atualiza modelo
    f->update(deltaT);

    touched = 0;

    //printf("updated\n");
    for (int i = 0; i < (*players).size(); i++) {
      touched = f->hasTouched((*players)[i]);
          // Verifica se tocou em algum bloco
      if(touched == 1){
        //soundManager->playKillSound(t0);
        (*players)[i]->resetPos();
        touched = 0; 
      }
    }
    // Atualiza tela
    //tela->update();

    for (int i=0; i<MAX_CONEXOES; i++) {
      c2 = key[i];
      if (c2=='w') {
        if((*players)[i]->getX() > 3)
          (*players)[i]->update((*players)[i]->getX()-1,(*players)[i]->getY());
      }
      if (c2=='a') {
        if((*players)[i]->getY() > 1)
          (*players)[i]->update((*players)[i]->getX(),(*players)[i]->getY()-1);
      }
      if (c2=='s') {
        if((*players)[i]->getX() < laneStartX+2)
          (*players)[i]->update((*players)[i]->getX()+1,(*players)[i]->getY());
      }
      if (c2=='d') {
        if((*players)[i]->getY() < laneY+2)
          (*players)[i]->update((*players)[i]->getX(),(*players)[i]->getY()+1);
      }
      if (c2=='q') {
        break;
      }
      key[i] = '0';
    }


   
    // // Verifica se atravessou as lanes
    // if(player->getX() <= (laneStartX-(l->getNumberOfLanes()))) {
    //   level++;
    //   nextLevel++;
    //   //soundManager->playLevelUpSound(t0);
    // }
    
    for (int i = 0; i < (*players).size(); i++) {
      if((*players)[i]->getX() <= (laneStartX-(l->getNumberOfLanes()))) {
        if (levelUpCount[i]==0) {
          (*players)[i]->levelUp();
          levelUpCount[i]=1;
        }
        //soundManager->playLevelUpSound(t0);
      }
    }

    int flag=1;
    for (int i=0; i<MAX_CONEXOES; i++) {
      flag=levelUpCount[i]*flag;
    }
    if (flag==1) {
      nextLevel++;
    }    

    // touched = f->hasTouched((*players)[i]);
    // // Verifica se tocou em algum bloco
    // if(touched == 1){
    //   //soundManager->playKillSound(t0);
    //   (*players)[i]->resetPos();
    //   touched = 0; 
    //   }
    // }

    /*printf("rel data!\n");
    rd = new RelevantData(*player, l, level);
    printf("serialize!\n");
    bufferStr = rd->serialize();
    printf("bufclear!\n");*/
    Json::Value root;
    Json::Value playerJson;
    Json::Value lanesJson;
    //player["char"] = 'A';
    //root["nivel"] = this->data->level;

    for (int i = 0; i < (*players).size(); i++) {
      playerJson["x"] = (*players)[i]->getX();
      playerJson["y"] = (*players)[i]->getY();
      root["player"][i] = playerJson;
    }
    
    // playerJson["x"] = player->getX();
    // playerJson["y"] = player->getY();
    
    vector<Lane*> *lvec = l->getLanes();
    int numcount = (*lvec).size();
    for(int i = 0; i < numcount; i++) {
        lanesJson[i]["x"] = (*lvec)[i]->getX();
        lanesJson[i]["pos"] = (*lvec)[i]->getPos();
        lanesJson[i]["content"] = (*lvec)[i]->getContent();
    }

    root["lanes"] = lanesJson;
    root["level"] = level;
    
    //etc

    Json::FastWriter fast;
    //Json::StyledWriter styled;
    bufferStr = fast.write(root);
    buffer.clear();
    buffer.resize(bufferStr.length() + 1);
    std::copy(bufferStr.c_str(), bufferStr.c_str() + bufferStr.length() + 1, buffer.begin());
    printf("%lu\n",buffer.size());
    //printf("RECEBI:\n%s\n", bufferStr.c_str());


    for (int i = 0; i < MAX_CONEXOES; i++) {
      if (send(connection_fd[i], &buffer[0], buffer.size(), MSG_NOSIGNAL) == -1) {
        printf("Usuario desconectou!\n");
      }
    }
    // if (send(connection_fd, &buffer[0], buffer.size(), MSG_NOSIGNAL) == -1) {
    //   printf("Usuario desconectou!\n");
    // }
    //printf("%s",bufferStr.c_str());

    // Condicao de parada
    if ( (t1-T) > 1000000 ) break;

    std::this_thread::sleep_for (std::chrono::milliseconds(100));

  }
  
  //tela->stop();
  teclado->stop();
  //tela->showSadFrog();
  return 0;
}
