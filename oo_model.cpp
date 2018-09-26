#include <vector>
#include <chrono>
#include <thread>
#include <iostream>
#include <time.h> 
#include <string>
#include <random>

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

Lane::Lane(int x, int nivel, int length) {
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
  std::random_device rd;
  std::mt19937 generator(rd());
  this->lanes = new std::vector<Lane *>(0);
  this->numberOflanes = 0;
  this->gen = generator;
}

/*void ListaDeLanes::hard_copy(ListaDeLanes *ldl) {
  std::vector<Lane *> *lanes = ldl->getLanes();

  for (int k=0; k<lanes->size(); k++) {
    Lane *l = new Lane( (*lanes)[k]->getX(),
                          (*lanes)[k]->getNivel() );
    this->addLane(l);
  }
}*/

std::mt19937 *ListaDeLanes::getGenerator() {
  return &(this->gen);
}

void ListaDeLanes::addLane(Lane *l) {
  (this->lanes)->push_back(l);
}

void ListaDeLanes::clearLanes() {
  (this->lanes)->clear();
}
void ListaDeLanes::createLanes(int maxNumLanes, int length, int startPos, int level) {

  std::normal_distribution<double> d(maxNumLanes/2,2.0);
  int numOfLanes = int(d(*(this->getGenerator())));
  int lastLevel;

  if(numOfLanes > level)
    numOfLanes = level;
  if(numOfLanes < level/2)
    numOfLanes = level/2;
  if(numOfLanes < 3)
    numOfLanes = 3;
  if(numOfLanes > maxNumLanes)
    numOfLanes = maxNumLanes; 

  this->numberOflanes = numOfLanes;

  for(int i = 0; i < numOfLanes; i++){
    Lane *tempLane = new Lane(startPos-i,level,length);
    (this->lanes)->push_back(tempLane);
  }
}

int ListaDeLanes::getNumberOfLanes() {
  return this->numberOflanes;
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

Tela::Tela(Player *player, ListaDeLanes *lanes, int *level, int maxI, int maxJ, float maxX, float maxY) {
  this->playerAtual = player;
  this->lanes = lanes;
  this->playerAnterior = new Player(0, 0);
  this->playerAnterior->update(player->getX(),player->getY());
  this->maxI = maxI;
  this->maxJ = maxJ;
  this->maxX = maxX;
  this->maxY = maxY;
  this->level = level;
}

void Tela::init() {
  initscr();			       /* Start curses mode 		*/
	raw();				         /* Line buffering disabled	*/
  curs_set(0);           /* Do not display cursor */

  //Desenha bordas da tela
  for(int i = 0; i < this->maxI+1; i++) {
    move(i,0);
    echochar('|');
    move(i,this->maxJ+1);
    echochar('|');
  }
  for(int i = 1; i < this->maxJ+1; i++) {
    move(0,i);
    echochar('-');
    move(2,i);
    echochar('-');
    move(this->maxI,i);
    echochar('-');
  }
}

void Tela::clearLaneArea() {
  for(int i = 5; i < this->maxI-2; i++) {
    for(int j = 1; j < this->maxJ; j++) {
      move(i,j);
      echochar(' ');
    }
  }
}

void Tela::update() {
  int playerI;
  int playerJ;

  int laneStartPos;
  int laneDrawPos;
  int lanePosOverflow;
  int n_cols;
  int n_lines;
  int numLanes;
  int numLines;
  int laneLength;

  getmaxyx(stdscr,n_lines,n_cols); /*size of the terminal*/

  std::vector<Lane *> *l = this->lanes->getLanes();
  numLanes = (*l).size();
  numLines = numLanes + 4 + 2 + 3;
  laneLength = (*l)[0]->content.size()+2;
  
  // escreve título e nível
  move(1,2);
  printw("FroggEEr  |  ");
  move(1,14);
  printw("          ");
  move(1,14);
  printw("Level: %d",*level);

  // Apaga lanes da tela
  for(int i = 0; i < (*l).size(); i++) {
      for(int j = 0; j<(*l)[i]->content.size(); j++) {
        move((*l)[i]->getX(),j+1);
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
      
      move((*l)[i]->getX(),laneDrawPos +1);
      echochar((*l)[i]->content[j]);
    }
  }

  // Apaga player na tela

  playerI = (int) (this->playerAnterior->getX()) * \
        (this->maxI / this->maxX);

  playerJ = (int) (this->playerAnterior->getY()) * \
        (this->maxJ / this->maxY);

  if((playerI < n_lines) && (playerJ < n_cols) && (playerI > 0) && (playerJ > 0)){ /*Check if inside the terminal window*/
    move(playerI, playerJ);   /* Move cursor to position */
    echochar(' ');  /* Prints character, advances a position */
  }

  // Desenha player na tela

  playerI = (int) (this->playerAtual->getX()) * \
        (this->maxI / this->maxX);

  playerJ = (int) (this->playerAtual->getY()) * \
        (this->maxJ / this->maxY);

  if((playerI < n_lines) && (playerJ < n_cols) && (playerI > 0) && (playerJ > 0)){ /*Check if inside the terminal window*/
    move(playerI, playerJ);   /* Move cursor to position */
    echochar('*');  /* Prints character, advances a position */
  }

  // Atualiza player
  this->playerAnterior->update(this->playerAtual->getX(), this->playerAtual->getY());

  // Atualiza tela
  refresh();
}

void Tela::stop() {
  endwin();
}

Tela::~Tela() {
  this->stop();;
}

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
