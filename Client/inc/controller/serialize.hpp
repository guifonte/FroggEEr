#ifndef SERIALIZE_HPP
#define SERIALIZE_HPP

#include "../model/lane.hpp"
#include "../model/player.hpp"

using namespace std;

class DataContainer {
  public:
    Player player;
    std::vector<Lane*> lanes;
    int numberOfLanes;
    int level;
};

class RelevantData {
  private:
    DataContainer *data;

  public:
    RelevantData();
    RelevantData(Player player, std::vector<Lane*> lanes, int level, int numberOfLanes);
    RelevantData(std::string buffer_in);
    void serialize(std::string &buffer_out);
    void unserialize(std::string buffer_in);
    DataContainer *dump();
};

#endif