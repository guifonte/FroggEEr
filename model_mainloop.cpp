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
  Player *player = new Player(10, 10);
  Lane *l1 = new Lane(9,1);
  Lane *l2 = new Lane(10,3);
  Lane *l3 = new Lane(11,5);
  Lane *l4 = new Lane(12,7);

  ListaDeLanes *l = new ListaDeLanes();
  l->add_corpo(l1);
  l->add_corpo(l2);
  l->add_corpo(l3);
  l->add_corpo(l4);
  
  Fisica *f = new Fisica(lane);

  Tela *tela = new Tela(player, lane, 50, 50, 50, 50);
  tela->init();

  Teclado *teclado = new Teclado();
  teclado->init();

  uint64_t t0;
  uint64_t t1;
  uint64_t deltaT;
  uint64_t T;

  int i = 0;
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
    // Atualiza tela
    tela->update();

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
    // Lê o teclado
   
    // Condicao de parada
    if ( (t1-T) > 1000000 ) break;

    std::this_thread::sleep_for (std::chrono::milliseconds(100));
    i++;
  }
  tela->stop();
  teclado->stop();
  return 0;
}
