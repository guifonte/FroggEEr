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
  /*Corpo *c1 = new Corpo(10, 30, 10, 100, 3);
  Corpo *c2 = new Corpo(10, 20, 10, 50, 0);
  Corpo *c3 = new Corpo(10, 50, 10, 20, 1.5);
  Corpo *c4 = new Corpo(10, 10, 10, 100, 0.3);
  Corpo *c5 = new Corpo(10, 15, 10, 80, 2);
  Corpo *c6 = new Corpo(10, 2, 10, 90, 0);
  Corpo *c7 = new Corpo(10, -15, 10, 15, 5);
  Corpo *c8 = new Corpo(10, -20, 10, 120, 10);

  ListaDeCorpos *l = new ListaDeCorpos();
  l->add_corpo(c1);
  l->add_corpo(c2);
  l->add_corpo(c3);
  l->add_corpo(c4);
  l->add_corpo(c5);
  l->add_corpo(c6);
  l->add_corpo(c7);
  l->add_corpo(c8);

  Fisica *f = new Fisica(l);*/

  Tela *tela = new Tela(player, 20, 20, 20, 20);
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
    //f->update(deltaT);

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
    /*char c = teclado->getchar();
    if (c=='s') {
      f->choque(forca);
    }
    if (c=='w') {
      f->choque(-forca);
    }
    if (c=='+') {
      forca += 100;
    }
    if (c=='-') {
      forca -= 100;
    }
    if (c=='q') {
      break;
    }*/

    // Condicao de parada
    if ( (t1-T) > 1000000 ) break;

    std::this_thread::sleep_for (std::chrono::milliseconds(100));
    i++;
  }
  tela->stop();
  teclado->stop();
  return 0;
}
