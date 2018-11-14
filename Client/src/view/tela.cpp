#include "../../inc/model/lane.hpp"
#include "../../inc/model/player.hpp"
#include "../../inc/view/tela.hpp"

#include <ncurses.h>

#define SAFEZONE_PAIR   1
#define STREET_PAIR     2
#define CAR_PAIR        3
#define P_BORDER_PAIR   4
#define P_STREET_PAIR   5
#define P_SAFEZONE_PAIR 6

Tela::Tela(ListaDePlayers *players, ListaDeLanes *lanes, int *level, int maxI, int maxJ, float maxX, float maxY) {
  this->playersAtuais = players;
  this->lanes = lanes;

  this->playersAnteriores = new ListaDePlayers();
  for(int i = 0; i <= this->playersAtuais->getNumOfPlayers(); i++){
    this->playersAnteriores->addPlayer(new Player(0, 0));
  }

  std::vector<Player *> *p = this->playersAtuais->getPlayers();
  std::vector<Player *> *p_ant = this->playersAnteriores->getPlayers();

  for(int i = 0; i < (*p).size(); i++) {
    (*p_ant)[i]->update((*p)[i]->getX(),(*p)[i]->getY());
  }
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
  noecho();              /* Do not output char to screen */

  /* initialize colors */

  start_color();
  init_pair(SAFEZONE_PAIR, COLOR_WHITE, COLOR_GREEN);
  init_pair(STREET_PAIR, COLOR_WHITE, COLOR_BLACK);
  init_pair(CAR_PAIR, COLOR_RED, COLOR_RED);
  init_pair(P_STREET_PAIR, COLOR_WHITE, COLOR_BLACK);
  init_pair(P_SAFEZONE_PAIR, COLOR_WHITE, COLOR_GREEN);
  init_pair(P_BORDER_PAIR, COLOR_WHITE, COLOR_WHITE);
  //Desenha bordas da tela
  attron(COLOR_PAIR(P_BORDER_PAIR));
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
  attroff(COLOR_PAIR(P_BORDER_PAIR));
}

void Tela::clearLaneArea() {
  attron(COLOR_PAIR(STREET_PAIR));
  for(int i = 5; i < this->maxI-2; i++) {
    for(int j = 1; j < this->maxJ+1; j++) {
      move(i,j);
      echochar(' ');
    }
  }
  attroff(COLOR_PAIR(STREET_PAIR));
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

  std::vector<Player *> *p = this->playersAtuais->getPlayers();
  std::vector<Player *> *p_ant = this->playersAnteriores->getPlayers();

  getmaxyx(stdscr,n_lines,n_cols); /*size of the terminal*/

  std::vector<Lane *> *l = this->lanes->getLanes();
  numLanes = (*l).size();
  numLines = numLanes + 4 + 2 + 3;
  laneLength = (*l)[0]->content.size()+2;
  
  // escreve título e nível
  move(1,2);
  printw("FroggEEr  ");
  move(1,11);
  attron(COLOR_PAIR(P_BORDER_PAIR));
  echochar('|');
  attroff(COLOR_PAIR(P_BORDER_PAIR));
  //move(1,3);
  //printw("          ");
  move(1,14);
  printw("Level: %d",*level);

  // Apaga lanes da tela
  for(int i = 0; i < (*l).size(); i++) {
      for(int j = 1; j<(*l)[i]->content.size()+3; j++) {
        move((*l)[i]->getX(),j);
        echochar(' ');
      }
  }

  //this->clearLaneArea();

  /* 
    Desenha lanes na tela.
    Para dar o efeito de movimento, sem modificar o vetor,
    se move em qual parte do vetor se começará à desenhar
    na parte inicial da tela, a partir do getPos().
    Ele é quem é atualizado a partir da velocidade em física::update
  */
  attron(COLOR_PAIR(CAR_PAIR));
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
      //echochar((*l)[i]->content[j]);
      if((*l)[i]->content[j]=='<'){
        echochar((*l)[i]->content[j]);
      }   
    }
  }
  attroff(COLOR_PAIR(CAR_PAIR));

  //Desenha safezones na tela

  attron(COLOR_PAIR(SAFEZONE_PAIR));
  for(int i = 1; i < this->maxJ+1; i++) {
    move(this->maxI-1,i);
    echochar(' ');
    move(this->maxI-2,i);
    echochar(' ');
    move(this->maxI-3-(*l).size(),i);
    echochar(' ');
    move(this->maxI-4-(*l).size(),i);
    echochar(' ');
  }
  attroff(COLOR_PAIR(SAFEZONE_PAIR));


  //------------------------------------------------------------------
  //-------------------------___________------------------------------
  //-------------------------| PLAYERS |------------------------------
  //-------------------------|_________|------------------------------
  //------------------------------------------------------------------

  //printf("size %d %d\n",(*p).size(),p->size());
  //printf("tam %d\n",this->playersAtuais->getNumOfPlayers());
  for(int i = 0; i < (*p).size(); i++) {

      // Apaga player na tela

    playerI = (int) ((*p_ant)[i]->getX()) * \
          (this->maxI / this->maxX);

    playerJ = (int) ((*p_ant)[i]->getY()) * \
          (this->maxJ / this->maxY);

    //printf("I J old: %d %d\n",playerI,playerJ);
    int safe = 0;
    if((playerI == this->maxI-1)||(playerI == this->maxI-2)||(playerI == this->maxI-3-(*l).size())||(playerI == this->maxI-4-(*l).size())){
      safe = 1;
      attron(COLOR_PAIR(P_SAFEZONE_PAIR));
    } else {
      attron(COLOR_PAIR(P_STREET_PAIR));
    }

    if((playerI < n_lines) && (playerJ < n_cols) && (playerI > 0) && (playerJ > 0)){ /*Check if inside the terminal window*/
      move(playerI, playerJ);   /* Move cursor to position */
      echochar(' ');  /* Prints character, advances a position */
    }

    if(safe == 1){
      attroff(COLOR_PAIR(P_SAFEZONE_PAIR));
    } else {
      attroff(COLOR_PAIR(P_STREET_PAIR));
    }

    // Desenha player na tela

    playerI = (int) ((*p)[i]->getX()) * \
          (this->maxI / this->maxX);

    playerJ = (int) ((*p)[i]->getY()) * \
          (this->maxJ / this->maxY);

    //printf("I J new: %d %d\n",playerI,playerJ);
    safe = 0;
    if((playerI == this->maxI-1)||(playerI == this->maxI-2)||(playerI == this->maxI-3-(*l).size())||(playerI == this->maxI-4-(*l).size())){
      attron(COLOR_PAIR(P_SAFEZONE_PAIR));
    } else {
      attron(COLOR_PAIR(P_STREET_PAIR));
    }

    if((playerI < n_lines) && (playerJ < n_cols) && (playerI > 0) && (playerJ > 0)){ /*Check if inside the terminal window*/
      move(playerI, playerJ);   /* Move cursor to position */
      echochar((*p)[i]->getAvatar());  /* Prints character, advances a position */
    }

    if(safe == 1){
      attroff(COLOR_PAIR(P_SAFEZONE_PAIR));
    } else {
      attroff(COLOR_PAIR(P_STREET_PAIR));
    }

    // Atualiza player
    (*p_ant)[i]->update((*p)[i]->getX(),(*p)[i]->getY());

  }
  

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
  //fclose(fptr);
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
    //fclose(fptr);
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
    //fclose(fptr);
}