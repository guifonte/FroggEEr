#include "../../inc/controller/fisica.hpp"
#include "../../inc/model/lane.hpp"
#include "../../inc/model/player.hpp"

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

/*
  verifica se o player tocou algum dos obstáculos
  como cada lane é um vetor, é apenas necessário verificar
  se a posição que o player está é a mesma que um < no vetor.
*/
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