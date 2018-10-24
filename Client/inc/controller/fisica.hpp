#ifndef FISICA_HPP
#define FISICA_HPP

#include "../model/lane.hpp"
#include "../model/player.hpp"

class Fisica {
  private:
    ListaDeLanes *lanes;
    Player *player;

  public:
    Fisica(ListaDeLanes *lanes, Player *player);
    int hasTouched();
    void update(float deltaT);
};

#endif