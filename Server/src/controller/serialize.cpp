#include "../../inc/controller/serialize.hpp"
#include "../../inc/model/lane.hpp"
#include "../../inc/model/player.hpp"

#include <iostream>
#include <fstream>
#include <jsoncpp/json/json.h> // or jsoncpp/json.h , or json/json.h etc.

#include <string>
#include <cstring>

using namespace std;

DataContainer::DataContainer() {
  this->lvec = new std::vector<Lane*>(0);
};

RelevantData::RelevantData() {
  this->data = new DataContainer();
};

RelevantData::RelevantData(Player player, ListaDeLanes *l, int level) {
  this->data->player = player;
  this->data->level = level;
  this->data->lvec = l->getLanes();
}

RelevantData::RelevantData(std::string buffer_in) {
  this->unserialize(buffer_in);
}

string RelevantData::serialize() {
  Json::Value root;
  Json::Value playerJson;
  Json::Value lanes;
  //player["char"] = 'A';
  //root["nivel"] = this->data->level;
  
  printf("playerX!\n");
  float x = this->data->player.getX();
  root["value"] = 10;
  printf("%lf\n",x);
  playerJson["x"] = x;
  
  
  printf("playerY!\n");
  playerJson["y"] = this->data->player.getY();
  printf("lanes!\n");
  int numcount = (*(this->data->lvec)).size();
  for(int i = 0; i < numcount; i++) {
      lanes[i]["x"] = ((*(this->data->lvec))[i])->getX();
      lanes[i]["pos"] = ((*(this->data->lvec))[i])->getPos();
      lanes[i]["content"] = ((*(this->data->lvec))[i])->getContent();
  }
  root["lanes"] = lanes;
  root["player"] = playerJson;
  //etc

  Json::FastWriter fast;
  Json::StyledWriter styled;
  string sFast = fast.write(root);
  return sFast;
}

void RelevantData::unserialize(std::string buffer_in) {
  std::memcpy(&(this->data), (void*)buffer_in.c_str(), sizeof(DataContainer));
}

DataContainer* RelevantData::dump() {
  return this->data;
}