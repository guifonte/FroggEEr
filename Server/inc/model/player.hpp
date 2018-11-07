#ifndef PLAYER_HPP
#define PLAYER_HPP

class Player {
  private:
  float startX;
  float startY;
  float x;
  float y;

  public:
  Player(float x, float y);
  void update(float newX,float newY);
  void resetPos();
  float getX();
  float getY();
};

#endif