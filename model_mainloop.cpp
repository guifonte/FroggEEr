#include <iostream>
#include <chrono>
#include <thread>
#include <vector>
#include <portaudio.h>

#include "01-playback.hpp"
#include "oo_model.hpp"

using namespace std::chrono;
uint64_t get_now_ms() {
  return duration_cast<milliseconds>(steady_clock::now().time_since_epoch()).count();
}

void playKillSound() {
  uint64_t t0, t1;
  Audio::Sample *asample;
  asample = new Audio::Sample();
  asample->load("res/blip.dat");
  Audio::SoundPlayer *soundPlayer;
  soundPlayer = new Audio::SoundPlayer();
  soundPlayer->init();
  soundPlayer->play(asample);
  t0 = get_now_ms();
  asample->set_position(0);
  while (1) {
    std::this_thread::sleep_for (std::chrono::milliseconds(1));
    t1 = get_now_ms();

    if (t1-t0 > 500) break;
  }
  soundPlayer->stop();
}

void playLevelUpSound() {
  uint64_t t0, t1;
  Audio::Sample *asample;
  asample = new Audio::Sample();
  asample->load("res/blip.dat");
  Audio::SoundPlayer *soundPlayer;
  soundPlayer = new Audio::SoundPlayer();
  soundPlayer->init();
  soundPlayer->play(asample);
  t0 = get_now_ms();
  asample->set_position(0);
  while (1) {
    std::this_thread::sleep_for (std::chrono::milliseconds(1));
    t1 = get_now_ms();

    if (t1-t0 > 1000) break;
  }
  asample->set_position(0);

  t0 = get_now_ms();
  while (1) {
    std::this_thread::sleep_for (std::chrono::milliseconds(1));
    t1 = get_now_ms();

    if (t1-t0 > 500) break;
  }

  soundPlayer->stop();
}

int main ()
{
  srand(time(NULL));

  int winX = 15;
  int winY = 51;

  showStartFrog();
  freopen("/dev/null", "w", stderr);

  //1 because of the botton besel and 2 because of the start safe zone
  int laneStartX = winX-3; 
  int laneY = winY-2; //2 because of the left and right besel
  //3 for the upper header, 1 for the botton besel and 4 for the safe zones
  //but there are winX+1 lines, because of that, -7
  int maxNumLanes = winX-7; 

  int level = 1;
  int nextLevel = 1 ; //boolean

  int touched = 0;
  float forca = 100;

  char c;
  char cPrev = 0;

  Player *player = new Player(laneStartX+2, laneY/2);
  ListaDeLanes *l = new ListaDeLanes();

  Fisica *f = new Fisica(l,player);

  Tela *tela = new Tela(player, l, &level ,winX, winY, winX, winY);
  tela->init();

  Teclado *teclado = new Teclado();
  teclado->init();

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

    if(nextLevel == 1) {
      nextLevel = 0;
      l->clearLanes();
      tela->clearLaneArea();
      l->createLanes(maxNumLanes,laneY,laneStartX, level);
      player->resetPos();
    }

    // Atualiza modelo
    f->update(deltaT);
    touched = f->hasTouched();

    // Atualiza tela
    tela->update();

    // Lê o teclado
    c = teclado->getchar();
    if (c != cPrev){
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

    // Verifica se tocou em algum bloco

    if(touched == 1){
      playKillSound();
      player->resetPos();
      touched = 0; 
    }
    
    // Verifica se atravessou as lanes
    if(player->getX() <= (laneStartX-(l->getNumberOfLanes()))) {
      level++;
      nextLevel++;
      playLevelUpSound();
    }

    // Condicao de parada
    if ( (t1-T) > 1000000 ) break;

    std::this_thread::sleep_for (std::chrono::milliseconds(100));

  }
  
  tela->stop();
  teclado->stop();
  
  return 0;
}
