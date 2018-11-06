#include "../../inc/controller/serialize.hpp"
#include "../../inc/model/lane.hpp"
#include "../../inc/model/player.hpp"

#include <string>
#include <cstring>

RelevantData::RelevantData() {
};


RelevantData::RelevantData(Player player, std::vector<Lane*> lanes, int level, int numberOfLanes) {
  this->data->player = player;
  this->data->lanes = lanes;
  this->data->level = level;
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