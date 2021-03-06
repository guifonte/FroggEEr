#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <chrono>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>

//model
#include "../inc/model/lane.hpp"
#include "../inc/model/player.hpp"
//view
#include "../inc/view/audioPlayer.hpp"
#include "../inc/view/tela.hpp"
#include "../inc/view/teclado.hpp"
//control
#include "../inc/controller/serialize.hpp"

#include <iostream>
#include <fstream>
#include <jsoncpp/json/json.h> // or jsoncpp/json.h , or json/json.h etc.

using namespace std::chrono;

uint64_t get_now_ms() {
  return duration_cast<milliseconds>(steady_clock::now().time_since_epoch()).count();
}

int main() {
  int socket_fd;
  struct sockaddr_in target;
  char serverIp[16];
  
  char playerName[10];
  char playerChar[2];
  char playerInfo[12];

  freopen("/dev/null", "w", stderr);

  printf("Bem vindo ao FroggEEr!\n");
  printf("Escreva o nome do seu jogador! (max 10 chars)\n");
  scanf("%s", playerName);	
  //printf("%s\n",playerName);
  printf("Escreva o char para o seu avatar!\n");
  scanf("%s", playerChar);	
  //printf("%s\n",playerChar);
  strcpy(playerInfo,playerChar);
  strcat(playerInfo,playerName);
  //printf("Infos: %s\n",playerInfo);

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
  printf("Esperando os outros players entrarem para jogar");
  //Manda char e nome
  send(socket_fd, &playerInfo, 12, 0);

  int winX = 15;
  int winY = 51;

  char c,c2;//char clicado no teclado
  char cPrev = '0';//char anterior clicado pelo teclado
  char key = '0';

  Teclado *teclado = new Teclado();
  teclado->init();

  char buffer[2048];
  int msg_len = 0;
  //RelevantData *rd = new RelevantData();
  //string buffer;
  //char *bufferchar = new char[buffer.length()+1];

  //std::vector<Lane*> *vecLanes;
  ListaDePlayers *p = new ListaDePlayers();
  ListaDeLanes *l = new ListaDeLanes();
  int level;
  int aux_level; //utilizado para saber se mudou de level
  float x;
  float y;
  int countLanes;
  int countPlayers;
  int laneX;
  float lanePos;
  string content;

  Audio::SoundManager *soundManager = new Audio::SoundManager("res/");
  int playKill = 0;
  int playLvlUp = 0;

  
  while(msg_len <= 0){
    msg_len = recv(socket_fd, buffer, 2048, MSG_DONTWAIT);
    if (msg_len > 0) {
      //printf("[%d] RECEBI:\n%s", msg_len, buffer);
      Json::Value root;
      Json::Reader reader;
      bool b = reader.parse(buffer, root);
      if (!b) {}
        //cout << "Error: " << reader.getFormattedErrorMessages();
      else {
        l->clearLanes();
        p->clearPlayers();
        level = root["level"].asLargestInt();
        aux_level = level;
        playKill = root["playKill"].asLargestInt();
        playLvlUp = root["playLvlUp"].asLargestInt();

        countPlayers = root["player"].size();
        for(int i = 0; i < countPlayers; i++){
          x = root["player"][i]["x"].asFloat();
          y = root["player"][i]["y"].asFloat();
          Player *tempPlayer = new Player(x,y);
          tempPlayer->setAvatar(char(root["player"][i]["avatar"].asLargestInt()));
          tempPlayer->setName(root["player"][i]["name"].asString());
          p->addPlayer(tempPlayer);
        }
        
        countLanes = root["lanes"].size();
        for(int i = 0; i < countLanes; i++){
          laneX = root["lanes"][i]["x"].asLargestInt();
          lanePos = root["lanes"][i]["pos"].asFloat();
          content = root["lanes"][i]["content"].asString();
          Lane *tempLane = new Lane(laneX, lanePos, content);
          l->addLane(tempLane);
        }
      }
    } else {
      //printf("msg_len =0\n");
      std::this_thread::sleep_for (std::chrono::milliseconds(100));
    }
  }

  //rd->unserialize(bufferchar);
  //DataContainer *data = rd->dump();

  //Tela *tela = new Tela(&(data->player), &(data->l), &(data->level) ,winX, winY, winX, winY);
  Tela *tela = new Tela(p,l,&level,winX, winY, winX, winY);
  //tela->showStartFrog();
  tela->init();
  tela->update();

  uint64_t t0;
  uint64_t t1;
  uint64_t deltaT;
  uint64_t T;

  T = get_now_ms();
  t1 = T;

  while (1) {

    //recv(socket_fd, bufferchar, strlen(bufferchar), 0);
    //rd->unserialize(bufferchar);
    msg_len = recv(socket_fd, buffer, 2048, MSG_DONTWAIT);
    if (msg_len > 0) {
      //printf("[%d] RECEBI:\n%s", msg_len, buffer);
      Json::Value root;
      Json::Reader reader;
      bool b = reader.parse(buffer, root);
      if (!b) {}
        //cout << "Error: " << reader.getFormattedErrorMessages();
      else {
        l->clearLanes();
        p->clearPlayers();
        level = root["level"].asLargestInt();
        playKill = root["playKill"].asLargestInt();
        playLvlUp = root["playLvlUp"].asLargestInt();

        countPlayers = root["player"].size();
        for(int i = 0; i < countPlayers; i++){
          x = root["player"][i]["x"].asFloat();
          y = root["player"][i]["y"].asFloat();
          Player *tempPlayer = new Player(x,y);
          tempPlayer->setAvatar(char(root["player"][i]["avatar"].asLargestInt()));
          tempPlayer->setName(root["player"][i]["name"].asString());
          p->addPlayer(tempPlayer);
        }
        
        countLanes = root["lanes"].size();
        for(int i = 0; i < countLanes; i++){
          laneX = root["lanes"][i]["x"].asLargestInt();
          lanePos = root["lanes"][i]["pos"].asFloat();
          content = root["lanes"][i]["content"].asString();
          Lane *tempLane = new Lane(laneX, lanePos, content);
          l->addLane(tempLane);
        }
      }
      //printf("%d\n",(p->getNumOfPlayers()));
    } else {
      //printf("msg_len =0");
    }

    // Atualiza timers
    t0 = t1;
    t1 = get_now_ms();
    deltaT = t1-t0;

    // Verifica se mudou o nível
    if(aux_level != level) {
      tela->clearLaneArea();
      aux_level = level;
    }

    // Atualiza tela
    tela->update();

        // // Verifica se tocou em algum bloco
    if(playKill == 1){
      //soundManager->playKillSound(t0);
      playKill = 0; 
    }
    if(playLvlUp == 1){
      //soundManager->playLevelUpSound(t0);
      playLvlUp = 0;
    }
    

    // Lê o teclado
    c = teclado->getchar();
    if (c != cPrev){ //evita que o usuário deixe o botão pressionado para andar mais rápido
      if (c=='w') {
        send(socket_fd, &c, 2, 0);
      }
      if (c=='a') {
        send(socket_fd, &c, 2, 0);
      }
      if (c=='s') {
        send(socket_fd, &c, 2, 0);
      }
      if (c=='d') {
        send(socket_fd, &c, 2, 0);
      }
      if (c=='q') {
        break;
      }
    }
    cPrev = c;

    std::this_thread::sleep_for (std::chrono::milliseconds(10));

  }
  
  tela->stop();
  teclado->stop();
  close(socket_fd);
  return 0;
}