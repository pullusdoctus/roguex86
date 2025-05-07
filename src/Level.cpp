#include <Level.hpp>

#include <stdexcept>

Level::Level() : roomCount(0), currentRoom(0) {
}

Level::~Level() {
  for (Room* room : this->rooms) {
    delete room;
  }
  this->rooms.clear();
}

Room Level::getRoom(int room) {
  if (room >= 0 && room < static_cast<int>(this->rooms.size())) {
    return *this->rooms[room];
  }
  throw std::out_of_range("Room index out of bounds");
}

Room* Level::getCurrentRoom() {
  if (this->currentRoom >= 0
    && this->currentRoom < static_cast<int>(this->rooms.size())) {
    return this->rooms[this->currentRoom];
  }
  throw std::out_of_range("Current room index out of bounds");
}

void Level::addRoom(Room* room) {
  this->rooms.push_back(room);
}

void Level::setRoomCount(int newRoomCount) {
  this->roomCount = newRoomCount;
}

int Level::getRoomCount() {
  return this->roomCount;
}
