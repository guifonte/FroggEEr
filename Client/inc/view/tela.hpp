#ifndef TELA_HPP
#define TELA_HPP

#include "../../inc/model/player.hpp"
#include "../../inc/model/lane.hpp"

class Tela {
  private:
    ListaDePlayers *playersAtuais, *playersAnteriores;
    ListaDeLanes *lanes;
    int *level;
    int maxI, maxJ;
    float maxX, maxY;
    void static delay(float number_of_seconds);

  public:
    Tela(ListaDePlayers *players, ListaDeLanes *lanes, int *level, int maxI, int maxJ, float maxX, float maxY);
    ~Tela();
    void stop();
    void init();
    void update();
    void clearLaneArea();
    void gameOver();
    void showSadFrog();
    void showStartFrog();
};

#endif