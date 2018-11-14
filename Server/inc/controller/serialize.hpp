#ifndef SERIALIZE_HPP
#define SERIALIZE_HPP

#include "../model/lane.hpp"
#include "../model/player.hpp"

#include <iostream>
#include <fstream>
#include <jsoncpp/json/json.h> // or jsoncpp/json.h , or json/json.h etc.
#include <cstring>

using namespace std;

class DataContainer {
  public:
    Player player;
    vector<Lane*> *lvec;
    int level;
    DataContainer();
};

class RelevantData {
  private:
    DataContainer *data;

  public:
    RelevantData();
    RelevantData(Player player, ListaDeLanes *l, int level);
    RelevantData(std::string buffer_in);
    string serialize();
    void unserialize(std::string buffer_in);
    DataContainer *dump();
};

#endif