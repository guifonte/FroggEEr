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

void Player::resetPos(){
  this->x = this->startX;
  this->y = this->startY;
}