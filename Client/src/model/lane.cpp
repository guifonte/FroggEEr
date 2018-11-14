#include "../../inc/model/lane.hpp"

#include <string>
#include <random>

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

/*
	Lane é o objeto de uma rua do frogger o qual o player
	precisa passar sem encostar em nenhum dos objetos.
*/
Lane::Lane(){
  
}
Lane::Lane(int x, float pos, string content){
  this->x = x;
  this->pos = pos;
  this->content = content;
}
Lane::Lane(int x, int nivel, int length, std::mt19937 *gen) {
  int makeGap = rand()%(2); // escolhe se a lane começa com uma lacuna ou com um obstáculo
  int tempLength;
  int filled = 0;
  this->x=x;
  this->pos=0;
  this->nivel=nivel;

  //escolhe a velocidade a partir de um valor randomico com probabilidade dada por uma distribuição normal
  std::normal_distribution<double> d_speed(nivel*2/3,nivel/6); 
  this->velocidade = 2*d_speed(*gen);

  //limita a velocidade mínima
  if(this->velocidade <= 3)
    this->velocidade = 3;

  //o tamanho dos obstaculos e das lacunas também são escolhidos a partir de distribuições normais
  std::normal_distribution<double> d_blocks(length/20*nivel*0.5,2);
  std::normal_distribution<double> d_gaps(length/2-nivel/2+4,2);

  while(filled < length) {
    if(makeGap == 1){ //faz lacuna
      makeGap = 0;
      tempLength = int(d_gaps(*gen));
      if(tempLength <= 3) //limita tamanho mínimo
        tempLength = 3;
      if(filled+tempLength > length) //verifica se extrapola o tamanho da lane
        tempLength = length - filled;
      for(int i = 0; i < tempLength; i++)
        this->content += " ";
    } else { //faz obstáculo
      makeGap = 1;
      tempLength = int(d_blocks(*gen));
      if(tempLength <= 4) //limita tamanho mínimo
        tempLength = 4;
      if(filled+tempLength > length) //verifica se extrapola o tamanho da lane
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
  //limita a quantidade máxima e mínima das lanes
  if(numOfLanes > level)
    numOfLanes = level;
  if(numOfLanes < level/3)
    numOfLanes = level/3;
  if(numOfLanes < 3)
    numOfLanes = 3;
  if(numOfLanes > maxNumLanes)
    numOfLanes = maxNumLanes; 

  this->numberOflanes = numOfLanes;

  //balanceia o nível máximo das lanes
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