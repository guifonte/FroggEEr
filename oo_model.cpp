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

Lane::Lane(int x, int nivel, int length, std::mt19937 *gen) {
  int makeGap = rand()%(2);
  int tempLength;
  int filled = 0;
  this->x=x;
  this->pos=0;
  this->nivel=nivel;

  std::normal_distribution<double> d_speed(nivel*2/3,nivel/6);
  this->velocidade = 2*d_speed(*gen);

  if(this->velocidade <= 3)
    this->velocidade = 3;

  std::normal_distribution<double> d_blocks(length/20*nivel*0.5,2);
  std::normal_distribution<double> d_gaps(length/2-nivel/2+4,2);

  while(filled < length) {
    if(makeGap == 1){ //make gap
      makeGap = 0;
      tempLength = int(d_gaps(*gen));
      if(tempLength <= 3)
        tempLength = 3;
      if(filled+tempLength > length)
        tempLength = length - filled;
      for(int i = 0; i < tempLength; i++)
        this->content += " ";
    } else { //make block
      makeGap = 1;
      tempLength = int(d_blocks(*gen));
      if(tempLength <= 4)
        tempLength = 4;
      if(filled+tempLength > length)
        tempLength = length - filled;
      for(int i = 0; i < tempLength; i++)
        this->content += "<";
    }
    filled += tempLength;
  }
}

ListaDeLanes::ListaDeLanes() {
  std::random_device rd;
  std::mt19937 generator(rd());
  this->lanes = new std::vector<Lane *>(0);
  this->numberOflanes = 0;
  this->gen = generator;
}

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
  int laneLevel;
  int maxLaneLevel;
  //balance the number of lanes
  if(numOfLanes > level)
    numOfLanes = level;
  if(numOfLanes < level/3)
    numOfLanes = level/3;
  if(numOfLanes < 3)
    numOfLanes = 3;
  if(numOfLanes > maxNumLanes)
    numOfLanes = maxNumLanes; 

  this->numberOflanes = numOfLanes;

  //balance the max level of lanes
  if(numOfLanes <= level/2) {
    maxLaneLevel = 2*level;
  }
  if(numOfLanes <= level/3) {
    maxLaneLevel = 3*level;
  }
  if(numOfLanes > level/2){
    maxLaneLevel = int(1.5*level);
  }
  if(numOfLanes >= 2*level/3){
    maxLaneLevel = level+1;
  }

  std::normal_distribution<double> distr_lanes(level,1.5);

  for(int i = 0; i < numOfLanes; i++){

    laneLevel = int(distr_lanes(*(this->getGenerator())));
    //balance the level of the lanes
    if(laneLevel == 0)
      laneLevel = 1;
    if(laneLevel > maxLaneLevel)
      laneLevel = maxLaneLevel;

    Lane *tempLane = new Lane(startPos-i,laneLevel,length,this->getGenerator());
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
  }
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
    addch('|');
    move(i,this->maxJ+1);
    addch('|');
  }
  for(int i = 1; i < this->maxJ+1; i++) {
    move(0,i);
    addch('-');
    move(2,i);
    addch('-');
    move(this->maxI,i);
    addch('-');
  }
}

void Tela::clearLaneArea() {
  for(int i = 5; i < this->maxI-2; i++) {
    for(int j = 1; j < this->maxJ; j++) {
      move(i,j);
      addch(' ');
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
      addch((*l)[i]->content[j]);
    }
  }

  // Apaga player na tela

  playerI = (int) (this->playerAnterior->getX()) * \
        (this->maxI / this->maxX);

  playerJ = (int) (this->playerAnterior->getY()) * \
        (this->maxJ / this->maxY);

  if((playerI < n_lines) && (playerJ < n_cols) && (playerI > 0) && (playerJ > 0)){ /*Check if inside the terminal window*/
    move(playerI, playerJ);   /* Move cursor to position */
    addch(' ');  /* Prints character, advances a position */
  }

  // Desenha player na tela

  playerI = (int) (this->playerAtual->getX()) * \
        (this->maxI / this->maxX);

  playerJ = (int) (this->playerAtual->getY()) * \
        (this->maxJ / this->maxY);

  if((playerI < n_lines) && (playerJ < n_cols) && (playerI > 0) && (playerJ > 0)){ /*Check if inside the terminal window*/
    move(playerI, playerJ);   /* Move cursor to position */
    addch('*');  /* Prints character, advances a position */
  }

  // Atualiza player
  this->playerAnterior->update(this->playerAtual->getX(), this->playerAtual->getY());

  // Atualiza tela
  refresh();
}

void Tela::gameOver() {
  FILE *fptr;

  if ((fptr = fopen("./res/sad_frog.txt","r")) == NULL){
    printf("Error! opening file");
    exit(1);
  }

  char c = fgetc(fptr); 
  move(0,0);
  int i=0;
  int j=0;

  while (c != EOF) { 
    if (c=='\0') {
      j++;
      i=0;
    }
    else {
      move(i,j);
      echochar(c); 
      refresh();
      i++;
    }
    //delay(0.3); 
    c = fgetc(fptr); 
  } 
  printf("\n");
  fclose(fptr);
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

void delay(float number_of_seconds) 
{ 
    // Converting time into milli_seconds 
    float milli_seconds = 1000 * number_of_seconds; 
  
    // Stroing start time 
    clock_t start_time = clock(); 
  
    // looping till required time is not acheived 
    while (clock() < start_time + milli_seconds); 
} 

void showStartFrog ()
{
    FILE *fptr;

    if ((fptr = fopen("./res/froggEEr.txt","r")) == NULL){
        printf("Error! opening file");
        exit(1);
    }

    char c = fgetc(fptr); 

    while (c != EOF) { 
        printf ("%c", c); 
        delay(0.3); 
        c = fgetc(fptr); 
    } 
    
    printf("\n");
    fclose(fptr);
    getchar();
}

void showSadFrog ()
{
    FILE *fptr;

    if ((fptr = fopen("./res/sad_frog.txt","r")) == NULL){
        printf("Error! opening file");
        exit(1);
    }

    //char c = fgetc(fptr); 

    char line[256];
    while (fgets(line, sizeof(line), fptr)) {
        /* note that fgets don't strip the terminating \n, checking its
           presence would allow to handle lines longer that sizeof(line) */
        printf("%s\n", line); 
    }

    // while (c != EOF) { 
    //     printf ("%c", c); 
    //     delay(0.3); 
    //     c = fgetc(fptr); 
    // } 
    
    printf("\n");
    fclose(fptr);
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
