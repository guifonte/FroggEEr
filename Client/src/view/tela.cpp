#include "../../inc/model/lane.hpp"
#include "../../inc/model/player.hpp"
#include "../../inc/view/tela.hpp"

#include <ncurses.h>

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

  /* 
    Desenha lanes na tela.
    Para dar o efeito de movimento, sem modificar o vetor,
    se move em qual parte do vetor se começará à desenhar
    na parte inicial da tela, a partir do getPos().
    Ele é quem é atualizado a partir da velocidade em física::update
  */
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

void Tela::delay(float number_of_seconds) 
{ 
    // Converting time into milli_seconds 
    float milli_seconds = 1000 * number_of_seconds; 
  
    // Stroing start time 
    clock_t start_time = clock(); 
  
    // looping till required time is not acheived 
    while (clock() < start_time + milli_seconds); 
} 

void Tela::showStartFrog(){
    FILE *fptr;

    if ((fptr = fopen("res/froggEEr.txt","r")) == NULL){
        printf("Error! opening file");
        exit(1);
    }

    char c = fgetc(fptr); 

    while (c != EOF) { 
        printf ("%c", c); 
        Tela::delay(0.3); 
        c = fgetc(fptr); 
    } 
    
    printf("\n");
    fclose(fptr);
    getchar();
}

void Tela::showSadFrog() {

    FILE *fptr;

    if ((fptr = fopen("res/sad_frog.txt","r")) == NULL){
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