#include "../../inc/model/player.hpp"

Player::Player() {
  
}

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

int Player::getLevel() {
  return this->level;
}

void Player::levelUp() {
  this->level++;
}


void Player::resetPos(){
  this->x = this->startX;
  this->y = this->startY;
}

ListaDePlayers::ListaDePlayers() {
  this->players = new std::vector<Player*>(0);
  this->numberOfplayers = 0;
}

void ListaDePlayers::addPlayer(Player *l) {
  (this->players)->push_back(l);
}

void ListaDePlayers::clearPlayers() {
  (this->players)->clear();
}

std::vector<Player*> *ListaDePlayers::getPlayers() {
  return (this->players);
}