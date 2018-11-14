#ifndef PLAYER_HPP
#define PLAYER_HPP
using namespace std;
#include <vector>

class Player {
  private:
  float startX;
  float startY;
  float x;
  float y;
  int level;

  public:
  Player();
  Player(float x, float y);
  void update(float newX,float newY);
  void resetPos();
  float getX();
  float getY();
  int getLevel();
  void levelUp();
};

class ListaDePlayers {
 private:
    std::vector<Player*> *players;
    int numberOfplayers;

  public:
    ListaDePlayers();
    void hard_copy(ListaDePlayers *lp);
    void addPlayer(Player *p);
    void clearPlayers();
    std::vector<Player*> *getPlayers();
};

#endif