/*
 * Arquivo: 	oo_model.hpp
 * Autores:	  Guilherme Nishino Fontebasso
 *		        Cynthia Baran
 * Descrição:	Implementação MVC de uma implementação do
 *		        jogo Frogger utilizando ALSA para os sons
 *		        e ncurses para a visualização
 * Comandos:	WASD para mover o asterisco (player)
 * 		        Q para sair
*/


#ifndef OO_MODEL_HPP
#define OO_MODEL_HPP

#include <thread>
#include <string>
#include <random>
#include <time.h>
#include <fstream>
#include <iostream>

using namespace std;

class Player {
  private:
  float startX;
  float startY;
  float x;
  float y;

  public:
  Player(float x, float y);
  void update(float newX,float newY);
  void resetPos();
  float getX();
  float getY();
};

class Lane {
  private:
  int x;
  int nivel;
  float pos;
  float velocidade;
  public:
  string content;
  Lane(int y, int nivel, int length, std::mt19937 *gen);
  void update(float newPos);
  void resetPos();
  float getPos();
  int getNivel();
  int getX();
  float getSpeed();
  string getContent();
};

class ListaDeLanes {
 private:
    std::vector<Lane*> *lanes;
    int numberOflanes;
    std::mt19937 gen;

  public:
    ListaDeLanes();
    void hard_copy(ListaDeLanes *ldl);
    void addLane(Lane *l);
    void clearLanes();
    void createLanes(int maxNumLanes, int length, int startPos, int level);
    std::mt19937 *getGenerator();
    int getNumberOfLanes();
    std::vector<Lane*> *getLanes();
};

class Fisica {
  private:
    ListaDeLanes *lanes;
    Player *player;

  public:
    Fisica(ListaDeLanes *lanes, Player *player);
    int hasTouched();
    void update(float deltaT);
};

class Tela {
  private:
    Player *playerAtual, *playerAnterior;
    ListaDeLanes *lanes;
    int *level;
    int maxI, maxJ;
    float maxX, maxY;

  public:
    Tela(Player *player, ListaDeLanes *lanes, int *level, int maxI, int maxJ, float maxX, float maxY);
    ~Tela();
    void stop();
    void init();
    void update();
    void clearLaneArea();
    void gameOver();

};

void threadfun (char *keybuffer, int *control);

class Teclado {
  private:
    char ultima_captura;
    int rodando;

    std::thread kb_thread;

  public:
    Teclado();
    ~Teclado();
    void stop();
    void init();
    char getchar();
};

void delay(float number_of_seconds);
void showStartFrog();
void showSadFrog();

class Server {
  public:
    Server();
    int init(unsigned int port);
    static void run(int *socket_fd, char *key, int *connection_fd);
};

class DataContainer {
  public:
    Player player;
    ListaDeLanes l;
    int level;
};

class RelevantData {
  private:
    DataContainer *data;

  public:
    RelevantData();
    RelevantData(Player player, ListaDeLanes l, int level);
    RelevantData(std::string buffer_in);
    void serialize(std::string &buffer_out);
    void unserialize(std::string buffer_in);
    DataContainer dump();
};

#endif
