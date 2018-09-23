#include <iostream>
#include <chrono>
#include <thread>
#include <vector>

#include "oo_model.hpp"

using namespace std::chrono;
uint64_t get_now_ms() {
  return duration_cast<milliseconds>(steady_clock::now().time_since_epoch()).count();
}

int main ()
{
  srand(time(NULL));
  Player *player = new Player(13, 25);
  Lane *l1 = new Lane(7,4);
  Lane *l2 = new Lane(8,2);
  Lane *l3 = new Lane(9,3);
  Lane *l4 = new Lane(10,2);
  Lane *l5 = new Lane(11,1);
  Lane *l6 = new Lane(12,1);

  ListaDeLanes *l = new ListaDeLanes();
  l->addLane(l1);
  l->addLane(l2);
  l->addLane(l3);
  l->addLane(l4);
  l->addLane(l5);
  l->addLane(l6);

  Fisica *f = new Fisica(l,player);

  Tela *tela = new Tela(player, l, 50, 50, 50, 50);
  tela->init();

  Teclado *teclado = new Teclado();
  teclado->init();

  uint64_t t0;
  uint64_t t1;
  uint64_t deltaT;
  uint64_t T;

  int i = 0;
  int touched = 0;
  float forca = 100;

  T = get_now_ms();
  t1 = T;
  while (1) {
    // Atualiza timers
    t0 = t1;
    t1 = get_now_ms();
    deltaT = t1-t0;
    // Atualiza modelo
    f->update(deltaT);
    touched = f->hasTouched();
    // Atualiza tela
    tela->update();

    // Lê o teclado
    char c = teclado->getchar();
    if (c=='w') {
      player->update(player->getX()-1,player->getY());
    }
    if (c=='a') {
      player->update(player->getX(),player->getY()-1);
    }
    if (c=='s') {
      player->update(player->getX()+1,player->getY());
    }
    if (c=='d') {
      player->update(player->getX(),player->getY()+1);
    }
    if (c=='q') {
      break;
    }
    
    if(touched == 1){
      player->resetPos();
      touched = 0;
    }
   
    // Condicao de parada
    if ( (t1-T) > 1000000 ) break;

    std::this_thread::sleep_for (std::chrono::milliseconds(100));
    i++;
  }
  tela->stop();
  teclado->stop();
  return 0;
}
