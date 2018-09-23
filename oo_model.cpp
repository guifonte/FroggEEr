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

void Player::resetPos(){
  this->x = this->startX;
  this->y = this->startY;
}

void Lane::update(float newPos) {
  this->pos=newPos;
}

float Lane::getPos() {
  return this->pos;
}

int Lane::getNivel() {
  return this->nivel;
}

float Lane::getSpeed() {
  return this->velocidade;
}

int Lane::getX() {
  return this->x;
}

string Lane::getContent() {
  return this->content;
}

Lane::Lane(int x, int nivel) {
  this->x=x;
  this->pos=0;
  this->nivel=nivel;

  int random = rand()%(10);
  switch(random) {
    case 0:
      this->content = "     <<<<<<<<          <<<<<<<<<       <<<<<<<<<<<";	
      this->velocidade = nivel * 5;
      break;
    case 1:
      this->content = "     <<<<              <<<             <<<<<<<    ";
      this->velocidade = nivel * 5;
      break;
    case 2:
      this->content = "     <<<<<<<<                          <<<<<<<<<<<";
      this->velocidade = nivel * 5;
      break;	
    case 3:
      this->content = "     <<<<<<<<                          <<<<<<<<<<<";
      this->velocidade = nivel * 1;
      break;	
    case 4:
      this->content = "                   <<<<<<<<<           <<<<<<<<<<<";
      this->velocidade = nivel * 1;
      break;	
    case 5:
      this->content = "     <<<<<<<<          <<<<<<<<<                  ";
      this->velocidade = nivel * 3;
      break;	
    case 6:
      this->content = "                                       <<<<<<<<<<<";
      this->velocidade = nivel * 3;
      break;
    case 7:
      this->content = "     <<<<<<<<         <<<<<<<<<<<<<<<<<<<<        ";
      this->velocidade = nivel * 2;
      break;	
    case 8:
      this->content = "          <<<<<<<<<<<          <<     <<<<<<<     ";
      this->velocidade = nivel * 2;
      break;	
    case 9:
      this->content = "     <<<<<<<<          <<<             <<<<<<<<<<<";
      this->velocidade = nivel * 2;
      break;
  }
}

ListaDeLanes::ListaDeLanes() {
  this->lanes = new std::vector<Lane *>(0);
}

void ListaDeLanes::hard_copy(ListaDeLanes *ldl) {
  std::vector<Lane *> *lanes = ldl->getLanes();

  for (int k=0; k<lanes->size(); k++) {
    Lane *l = new Lane( (*lanes)[k]->getX(),
                          (*lanes)[k]->getNivel() );
    this->addLane(l);
  }
}

void ListaDeLanes::addLane(Lane *l) {
  (this->lanes)->push_back(l);
}

std::vector<Lane*> *ListaDeLanes::getLanes() {
  return (this->lanes);
}

Fisica::Fisica(ListaDeLanes *lanes, Player *player) {
  this->lanes = lanes;
  this->player = player;
}

void Fisica::update(float deltaT) {
  // Atualiza parametros das lanes!
  std::vector<Lane *> *l = this->lanes->getLanes();
  for(int i = 0; i < (*l).size(); i++) {
    float newPos = (*l)[i]->getPos() + (float)deltaT * (*l)[i]->getSpeed()/1000;
    if(newPos>50){
      newPos=newPos-50;
    }
    (*l)[i]->update(newPos);
    //printf("%d %f / ",(*l)[i]->getX() ,(*l)[i]->getPos());
  }
  //printf("\n");
}

int Fisica::hasTouched(){
  std::vector<Lane *> *l = this->lanes->getLanes();
  for(int i = 0; i < (*l).size(); i++) {
    if((*l)[i]->getX() == player->getX()){
      if(player->getY()>=(*l)[i]->getPos()){
        if((*l)[i]->getContent()[(int)player->getY() - (*l)[i]->getPos()] == '<'){
          return 1;
        } else {
          return 0;
        }
      } else {
        if((*l)[i]->getContent()[(*l)[i]->content.size() - (*l)[i]->getPos() + (int)player->getY()] == '<'){
          return 1;
        } else {
          return 0;
        }
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

Tela::Tela(Player *player, ListaDeLanes *lanes, int maxI, int maxJ, float maxX, float maxY) {
  this->playerAtual = player;
  this->lanes = lanes;
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

  int laneStartPos;
  int laneDrawPos;
  int lanePosOverflow;
  int n_cols;
  int n_lines;

  getmaxyx(stdscr,n_lines,n_cols); /*size of the terminal*/


  // Apaga lanes da tela
  std::vector<Lane *> *l = this->lanes->getLanes();
  for(int i = 0; i < (*l).size(); i++) {
      for(int j = 0; j<(*l)[i]->content.size(); j++) {
        move((*l)[i]->getX(),j);
        echochar(' ');
      }
  }


  // Desenha lanes na tela
  for(int i = 0; i < (*l).size(); i++) {
    laneStartPos = (*l)[i]->getPos();
    laneDrawPos = laneStartPos;
    lanePosOverflow = 0;

    for(int j = 0; j<(*l)[i]->content.size(); j++) {
      if(laneDrawPos >= (*l)[i]->content.size()) {
        lanePosOverflow = 1;
        laneDrawPos = 0;
      } 

      if(lanePosOverflow == 0) {
        laneDrawPos = j + laneStartPos;
      } else {
        laneDrawPos++;
      }
      
      move((*l)[i]->getX(),laneDrawPos);
      echochar((*l)[i]->content[j]);
    }
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
