#include "../../inc/controller/serialize.hpp"
#include "../../inc/model/lane.hpp"
#include "../../inc/model/player.hpp"

#include <string>
#include <cstring>

RelevantData::RelevantData() {
};


RelevantData::RelevantData(Player player, Lane lanes[], int level, int numberOfLanes) {
  printf("Pre player\n");
  this->data->player = player;
  printf("Pre lanes\n");
  for(int i = 0; i < numberOfLanes;i++){
    this->data->lanes[i] = *(new Lane());
    printf("lane %d de %d\n",i,numberOfLanes);
    this->data->lanes[i] = lanes[i];
  }
  printf("Pre level\n");
  this->data->level = level;
  printf("Pre numoflanes\n");
  this->data->numberOfLanes = numberOfLanes;
}

RelevantData::RelevantData(std::string buffer_in) {
  this->unserialize(buffer_in);
}

void RelevantData::serialize(std::string &buffer_out) {
  std::memcpy((void*)buffer_out.c_str(), &(this->data), sizeof(DataContainer));
}

void RelevantData::unserialize(std::string buffer_in) {
  std::memcpy(&(this->data), (void*)buffer_in.c_str(), sizeof(DataContainer));
}

DataContainer* RelevantData::dump() {
  return this->data;
}