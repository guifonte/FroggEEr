#ifndef LANE_HPP
#define LANE_HPP

#include <string>
#include <random>

using namespace std;

class Lane {
  private:
  int x;
  int nivel;
  float pos;
  float velocidade;
  
  public:
  string content;
  Lane();
  Lane(int y, int nivel, int length, std::mt19937 *gen);
  void update(float newPos);
  void resetPos();
  float getPos();
  int getNivel();
  int getX();
  float getSpeed();
  string getContent();
};

class ListaDeLanes {
 private:
    std::vector<Lane*> *lanes;
    int numberOflanes;
    std::mt19937 gen;

  public:
    ListaDeLanes();
    void hard_copy(ListaDeLanes *ldl);
    void addLane(Lane *l);
    void clearLanes();
    void createLanes(int maxNumLanes, int length, int startPos, int level);
    std::mt19937 *getGenerator();
    int getNumberOfLanes();
    std::vector<Lane*> *getLanes();
};

#endif