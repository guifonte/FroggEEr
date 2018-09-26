
#ifndef OO_MODEL_HPP
#define OO_MODEL_HPP

#include <thread>
#include <string>
#include <random>

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
  Lane(int y, int nivel, int length);
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
    //void add_lane(Corpo *c);
    int hasTouched();
    void update(float deltaT);
};

class Tela {
  private:
    //ListaDeCorpos *lista, *lista_anterior;
    Player *playerAtual, *playerAnterior;
    ListaDeLanes *lanes;
    int *level;
    int maxI, maxJ;
    float maxX, maxY;

  public:
    //Tela(ListaDeCorpos *ldc, int maxI, int maxJ, float maxX, float maxY);
    Tela(Player *player, ListaDeLanes *lanes, int *level, int maxI, int maxJ, float maxX, float maxY);
    ~Tela();
    void stop();
    void init();
    void update();
    void clearLaneArea();
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

#endif