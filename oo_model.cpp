#include <vector>
#include <chrono>
#include <thread>
#include <iostream>
#include <time.h> 
#include <string>

#include "oo_model.hpp"

#include <ncurses.h>
using namespace std::chrono;

Player::Player(float x, float y) {
  this->startX = x;
  this->startY = y;
  this->x = x;
  this->y = y;
}

void Player::update(float newX, float newY) {
  this->x = newX;
  this->y = newY;
}

float Player::getX() {
  return this->x;
}

float Player::getY() {
  return this->y;
}

void Lane::update(float newPos) {
  this->pos=newPos;
}

float Lane::getPos() {
  return this->pos;
}

float Lane::getSpeed() {
  return this->velocidade;
}

int Lane::getX() {
  return this->x;
}

Lane::Lane(int x, int nivel) {
  this->x=x;
  this->pos=0;

  srand(time(NULL));
  int random = rand()%(10);
  switch(random) {
    case 0:
      this->content = "     <<<<<<<<          <<<<<<<<<       <<<<<<<<<<<";	
      this->velocidade = nivel * 10;
      break;
    case 1:
      this->content = "     <<<<              <<<             <<<<<<<    ";
      this->velocidade = nivel * 10;
      break;
    case 2:
      this->content = "     <<<<<<<<                          <<<<<<<<<<<";
      this->velocidade = nivel * 10;
      break;	
    case 3:
      this->content = "     <<<<<<<<                          <<<<<<<<<<<";
      this->velocidade = nivel * 2;
      break;	
    case 4:
      this->content = "                   <<<<<<<<<           <<<<<<<<<<<";
      this->velocidade = nivel * 2;
      break;	
    case 5:
      this->content = "     <<<<<<<<          <<<<<<<<<                  ";
      this->velocidade = nivel * 6;
      break;	
    case 6:
      this->content = "                                       <<<<<<<<<<<";
      this->velocidade = nivel * 6;
      break;
    case 7:
      this->content = "     <<<<<<<<         <<<<<<<<<<<<<<<<<<<<        ";
      this->velocidade = nivel * 4;
      break;	
    case 8:
      this->content = "          <<<<<<<<<<<          <<     <<<<<<<     ";
      this->velocidade = nivel * 4;
      break;	
    case 9:
      this->content = "     <<<<<<<<          <<<             <<<<<<<<<<<";
      this->velocidade = nivel * 4;
      break;
  }
}

/*Corpo::Corpo(float massa, float velocidade, float posicao, float elasticidade, float amortecimento) {
  this->massa = massa;
  this->velocidade = velocidade;
  this->posicao = posicao;
  this->posicao_central = posicao;
  this->elasticidade = elasticidade;
  this->amortecimento = amortecimento;
  this->forca = 0;
}

void Corpo::update(float nova_velocidade, float nova_posicao, float nova_forca) {
  this->velocidade = nova_velocidade;
  this->posicao = nova_posicao;
  this->forca = nova_forca;
}

float Corpo::get_massa() {
  return this->massa;
}

float Corpo::get_velocidade() {
  return this->velocidade;
}

float Corpo::get_posicao() {
  return this->posicao;
}

float Corpo::get_posicao_central() {
  return this->posicao_central;
}

float Corpo::get_elasticidade() {
  return this->elasticidade;
}

float Corpo::get_amortecimento() {
  return this->amortecimento;
}

float Corpo::get_forca() {
  return this->forca;
}

ListaDeCorpos::ListaDeCorpos() {
  this->corpos = new std::vector<Corpo *>(0);
}

void ListaDeCorpos::hard_copy(ListaDeCorpos *ldc) {
  std::vector<Corpo *> *corpos = ldc->get_corpos();

  for (int k=0; k<corpos->size(); k++) {
    Corpo *c = new Corpo( (*corpos)[k]->get_massa(),\
                          (*corpos)[k]->get_posicao(),\
                          (*corpos)[k]->get_velocidade(),\
                          (*corpos)[k]->get_elasticidade(),\
                          (*corpos)[k]->get_amortecimento()
                        );
    this->add_corpo(c);
  }
}


void ListaDeCorpos::add_corpo(Corpo *c) {
  (this->corpos)->push_back(c);
}

std::vector<Corpo*> *ListaDeCorpos::get_corpos() {
  return (this->corpos);
}
*/
Fisica::Fisica(Lane *lane, Player *player) {
  this->lane = lane;
  this->player = player;
}

void Fisica::update(float deltaT) {
  // Atualiza parametros dos corpos!
  float newPos = this->lane->getPos() + (float)deltaT * this->lane->getSpeed()/1000;
  if(newPos>50){
    newPos=newPos-50;
  }
  lane->update(newPos);
}

int Fisica::hasTouched() {
  
  std::vector<Lane *> *l = this->lanes->getLanes();
  for (int i = 0; i < (*l).size(); i++) {
    if((*l)[i]->getX() == player->getX()){
      if((*l)[i]->getContent()[(int)player->getY()] == '<'){
        return 1;
      } else {
        return 0;
      }
    }
  }
  return 0;
}

/*Tela::Tela(ListaDeCorpos *ldc, int maxI, int maxJ, float maxX, float maxY) {
  this->lista = ldc;
  this->lista_anterior = new ListaDeCorpos();
  this->lista_anterior->hard_copy(this->lista);
  this->maxI = maxI;
  this->maxJ = maxJ;
  this->maxX = maxX;
  this->maxY = maxY;
}*/

Tela::Tela(Player *player, Lane *lane, int maxI, int maxJ, float maxX, float maxY) {
  this->playerAtual = player;
  this->lane = lane;
  this->playerAnterior = new Player(0, 0);
  this->playerAnterior->update(player->getX(),player->getY());
  this->maxI = maxI;
  this->maxJ = maxJ;
  this->maxX = maxX;
  this->maxY = maxY;
}

void Tela::init() {
  initscr();			       /* Start curses mode 		*/
	raw();				         /* Line buffering disabled	*/
  curs_set(0);           /* Do not display cursor */
}

void Tela::update() {
  int playerI;
  int playerJ;

  int laneStartPos = this->lane->getPos();
  int laneDrawPos = laneStartPos;
  int lanePosOverflow = 0;
  int n_cols;
  int n_lines;

  getmaxyx(stdscr,n_lines,n_cols); /*size of the terminal*/

  //std::vector<Corpo *> *corpos_old = this->lista_anterior->get_corpos();

  // Apaga corpos na tela
  //for (int k=0; k<corpos_old->size(); k++)
  //{
  //  i = (int) ((*corpos_old)[k]->get_posicao()) * \
  //      (this->maxI / this->maxX);
  //
  //  if((i < n_lines) && (k < n_cols) && (i > 0)){ /*Check if inside the terminal window*/
  //    move(i, k);   /* Move cursor to position */
  //    echochar(' ');  /* Prints character, advances a position */
  //  }
  //}

  // Apaga lane da tela

  for(int i = 0; i<this->lane->content.size(); i++) {
    move(this->lane->getX(),i);
    echochar(' ');
  }

  // Desenha lane na tela
  for(int i = 0; i<this->lane->content.size(); i++) {
    if(laneDrawPos>=this->lane->content.size()) {
      lanePosOverflow = 1;
      laneDrawPos = 0;
    } 

    if(lanePosOverflow == 0) {
      laneDrawPos = i + laneStartPos;
    } else {
      laneDrawPos++;
    }
    
    move(this->lane->getX(),laneDrawPos);
    echochar(this->lane->content[i]);
  }

  // Apaga player na tela

  playerI = (int) (this->playerAnterior->getX()) * \
        (this->maxI / this->maxX);

  playerJ = (int) (this->playerAnterior->getY()) * \
        (this->maxJ / this->maxY);

  /*for(int i=10;i>=0;i--){
    move(0, i);
    echochar(' ');
  }
  printf("%d %d\n",playerI,playerJ);*/

  if((playerI < n_lines) && (playerJ < n_cols) && (playerI > 0) && (playerJ > 0)){ /*Check if inside the terminal window*/
    move(playerI, playerJ);   /* Move cursor to position */
    echochar(' ');  /* Prints character, advances a position */
  }

  // Desenha player na tela

  playerI = (int) (this->playerAtual->getX()) * \
        (this->maxI / this->maxX);

  playerJ = (int) (this->playerAtual->getY()) * \
        (this->maxJ / this->maxY);

  /*for(int i=10;i>=0;i--){
    move(1, i);
    echochar(' ');
  }
  printf("%d %d\n",playerI,playerJ);*/

  if((playerI < n_lines) && (playerJ < n_cols) && (playerI > 0) && (playerJ > 0)){ /*Check if inside the terminal window*/
    move(playerI, playerJ);   /* Move cursor to position */
    echochar('*');  /* Prints character, advances a position */
  }

  // Atualiza player
  
  this->playerAnterior->update(this->playerAtual->getX(), this->playerAtual->getY());
  /*printf("%f %f ", this->playerAnterior->getX(), this->playerAnterior->getY());*/
  // Desenha corpos na tela
  //std::vector<Corpo *> *corpos = this->lista->get_corpos();

  //for (int k=0; k<corpos->size(); k++)
  //{
  //  i = (int) ((*corpos)[k]->get_posicao()) * \
  //      (this->maxI / this->maxX);
  //
  //  if((i < n_lines) && (k < n_cols) && (i > 0)){ /*Check if inside the terminal window*/
  //    move(i, k);   /* Move cursor to position */
  //    echochar('*');  /* Prints character, advances a position */
  //  }


    // Atualiza corpos antigos
  //  (*corpos_old)[k]->update(  (*corpos)[k]->get_velocidade(),\
  //                             (*corpos)[k]->get_posicao(),\
  //                             (*corpos)[k]->get_forca());
  //}

  // Atualiza tela
  refresh();
}

void Tela::stop() {
  endwin();
}

Tela::~Tela() {
  this->stop();;
}


/*
class Teclado {
  private:
    char ultima_captura;
    int rodando;

  public:
    Teclado();
    ~Teclado();
    void stop();
    void init();
    char getchar();
}

*/

void threadfun (char *keybuffer, int *control)
{
  char c;
  while ((*control) == 1) {
    c = getch();
    if (c!=ERR) (*keybuffer) = c;
    else (*keybuffer) = 0;
    std::this_thread::sleep_for (std::chrono::milliseconds(10));
  }
  return;
}

Teclado::Teclado() {
}

Teclado::~Teclado() {
}

void Teclado::init() {
  // Inicializa ncurses
  raw();				         /* Line buffering disabled	*/
	keypad(stdscr, TRUE);	 /* We get F1, F2 etc..		*/
	noecho();			         /* Don't echo() while we do getch */
  curs_set(0);           /* Do not display cursor */

  this->rodando = 1;
  std::thread newthread(threadfun, &(this->ultima_captura), &(this->rodando));
  (this->kb_thread).swap(newthread);
}

void Teclado::stop() {
  this->rodando = 0;
  (this->kb_thread).join();
}

char Teclado::getchar() {
  char c = this->ultima_captura;
  this->ultima_captura = 0;
  return c;
}
