#ifndef FISICA_HPP
#define FISICA_HPP

#include "../model/lane.hpp"
#include "../model/player.hpp"

class Fisica {
  private:
    ListaDeLanes *lanes;
    ListaDePlayers *players;

  public:
    Fisica(ListaDeLanes *lanes, ListaDePlayers *players);
    int hasTouched(Player *player);
    void update(float deltaT);
};

#endif