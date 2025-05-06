#include <Room.cpp>

Level::Level() : roomCount(0), currentRoom(0) {
}

Level::~Level() {
  this->rooms.clear();
}

Room Level::getRoom(int room) {
  return rooms[room];
}

void Level::addRoom(Room& room) {
  this->rooms.push_back(room);
}

void Level::setRoomCount(int newRoomCount) {
  this->roomCount = newRoomCount;
}

int Level::getRoomCount() {
  return this->roomCount;
}
