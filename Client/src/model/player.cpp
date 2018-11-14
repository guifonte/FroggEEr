#include "../../inc/model/player.hpp"

#include <string>
#include <cstring>

Player::Player() {
  
}

Player::Player(float x, float y) {
  this->startX = x;
  this->startY = y;
  this->x = x;
  this->y = y;
  this->avatar = '0';
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

void Player::setAvatar(char avatar){
  this->avatar = avatar;
}

void Player::setName(char name[]){
  strcpy(this->name,name);
}

ListaDePlayers::ListaDePlayers() {
  this->players = new std::vector<Player*>(0);
}

int ListaDePlayers::getNumOfPlayers(){
  return (this->players)->size();
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

