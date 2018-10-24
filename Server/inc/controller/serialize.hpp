#ifndef SERIALIZE_HPP
#define SERIALIZE_HPP

#include "../model/lane.hpp"
#include "../model/player.hpp"

class DataContainer {
  public:
    Player player;
    ListaDeLanes l;
    int level;
};

class RelevantData {
  private:
    DataContainer *data;

  public:
    RelevantData();
    RelevantData(Player player, ListaDeLanes l, int level);
    RelevantData(std::string buffer_in);
    void serialize(std::string &buffer_out);
    void unserialize(std::string buffer_in);
    DataContainer dump();
};

#endif