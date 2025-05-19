#include <Level.hpp>

#include <iostream>
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

bool Level::connectRooms() {
  if (rooms.size() <= 1) return false;
  // track which rooms have a connection to another room
  std::vector<bool> roomConnected(rooms.size(), false);
  // check if a room has adjacent rooms
  std::cout << "checking room adjacency..." << std::endl;
  std::cout << "room count: " << rooms.size() << std::endl;
  for (size_t i = 0; i < rooms.size(); ++i) {
    Room* room = rooms[i];
    for (Direction dir = NORTH; dir <= EAST; ++dir) {
      if (room->getAdjacentRoom(dir) != nullptr) {
        roomConnected[i] = true;
        break;
      }
    }
  }
  std::cout << "done checking room adjacency" << std::endl;
  // connect rooms without connections
  bool changeMade;
  do {
    changeMade = false;
    for (size_t roomAIndex = 0; roomAIndex < rooms.size(); ++roomAIndex) {
      if (roomConnected[roomAIndex]) continue;  // ignore rooms w/ connections
      // connect to other rooms
      Room* roomA = rooms[roomAIndex];
      bool roomAConnected = false;
      // check all four directions
      for (Direction dir = NORTH; dir <= EAST; ++dir) {
        // don't connect on a direction with an existing connection
        if (roomA->getAdjacentRoom(dir) != nullptr) continue;
        // find a room to connect to
        for (size_t roomBIndex = 0; roomBIndex < rooms.size() &&
        !roomAConnected; ++roomBIndex) {
          if (roomAIndex == roomBIndex) continue;  // don't connect with itself
          Room* roomB = rooms[roomBIndex];
          Direction oppDir = roomA->getOppositeDirection(dir);
          // skip if room B has a connection in the opposite direction
          if (roomB->getAdjacentRoom(oppDir) != nullptr) continue;
          // connect the rooms
          roomA->connect(dir, roomB);
          roomB->connect(oppDir, roomA);
          // create doorways
          roomA->createDoorway(dir);
          roomB->createDoorway(oppDir);

          roomAConnected = true;
          roomConnected[roomAIndex] = true;
          roomConnected[roomBIndex] = true;
          changeMade = true;
        }
      }
    }
  } while (changeMade);
  // check if all rooms were connected
  for (size_t i = 0; i < rooms.size(); ++i) {
    if (!roomHasConnection(i)) {
      return false;  // at least one room couldn't be connected
    }
  }
  return true;
}

bool Level::roomHasConnection(int i) {
  for (Direction dir = NORTH; dir <= EAST; ++dir) {
    if (this->rooms[i]->getAdjacentRoom(dir) != nullptr) return true;
  }
  return false;
}
