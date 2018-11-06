#ifndef SERIALIZE_HPP
#define SERIALIZE_HPP

#include "../model/lane.hpp"
#include "../model/player.hpp"

class DataContainer {
  public:
    Player player;
    Lane lanes[8];
    int numberOfLanes;
    int level;
};

class RelevantData {
  private:
    DataContainer *data;

  public:
    RelevantData();
    RelevantData(Player player, Lane lanes[], int level, int numberOfLanes);
    RelevantData(std::string buffer_in);
    void serialize(std::string &buffer_out);
    void unserialize(std::string buffer_in);
    DataContainer *dump();
};

#endif