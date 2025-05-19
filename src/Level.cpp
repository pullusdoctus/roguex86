#include <Level.hpp>

#include <algorithm>
#include <iostream>
#include <random>
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
  return this->currentRoom;
}

void Level::addRoom(Room* room) {
  if (!this->currentRoom) this->currentRoom = room;
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
  for (size_t i = 0; i < rooms.size(); ++i) {
    Room* room = rooms[i];
    for (Direction dir = NORTH; dir <= EAST; ++dir) {
      if (room->getAdjacentRoom(dir) != nullptr) {
        roomConnected[i] = true;
        break;
      }
    }
  }
  // connect rooms without connections
  bool changeMade;
  bool roomsRemoved = false;
  // store all directions to try and connect randomly
  std::vector<Direction> directions = {NORTH, SOUTH, WEST, EAST};
  do {
    changeMade = false;
    // try to connect each room at least once
    for (size_t roomAIndex = 0; roomAIndex < rooms.size(); ++roomAIndex) {
      if (roomConnected[roomAIndex]) continue;  // ignore rooms w/ connections
      // connect to other rooms
      Room* roomA = rooms[roomAIndex];
      bool roomAConnected = false;
      // get a random direction to try and connect from
      std::random_device rd;
      std::mt19937 g(rd());
      std::shuffle(directions.begin(), directions.end(), g);
      // to check if this direction has already been tried
      std::vector<Direction> tried;
      // check all four directions
      for (Direction dir : directions) {
        // don't connect on a direction with an existing connection
        if (roomA->getAdjacentRoom(dir) != nullptr) continue;
        tried.push_back(dir);
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
      // if a connection couldn't be made for room A
      if (!roomAConnected) {
        // double check it doesn't have connections
        bool hasConnections = false;
        for (Direction dir = NORTH; dir <= EAST; ++dir) {
          if (roomA->getAdjacentRoom(dir)) {
            hasConnections = true;
            break;
          }
        }
        // after double checking, if it actually isn't connected to another room
        if (!hasConnections) {
          // delete the room from memory
          delete this->rooms[roomAIndex];
          this->rooms.erase(this->rooms.begin() + roomAIndex);
          roomConnected.erase(roomConnected.begin() + roomAIndex);
          roomAIndex--;  // adjust index after removal
          roomsRemoved = true;
          changeMade = true;
        }
      }
    }
  } while (changeMade);
  if (roomsRemoved) this->roomCount = this->rooms.size();
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

void Level::moveRoom(Room* nextRoom) {
  for (Room* room : this->rooms) {
    if (room == nextRoom) {
      this->currentRoom = nextRoom;
      return;
    }
  }
}
