#include "../../inc/controller/serialize.hpp"
#include "../../inc/model/lane.hpp"
#include "../../inc/model/player.hpp"

#include <string>
#include <cstring>

RelevantData::RelevantData() {
};


RelevantData::RelevantData(Player player, ListaDeLanes l, int level) {
  this->data->player = player;
  this->data->l = l;
  this->data->level = level;
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

DataContainer RelevantData::dump() {
  return *(this->data);
}