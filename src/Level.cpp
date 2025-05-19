#include <Level.hpp>

#include <algorithm>
#include <asm.h>
#include <iostream>
#include <Macros.h>
#include <random>
#include <stdexcept>

Level::Level() : roomCount(0), currentRoom(0), staircaseX(-1), staircaseY (-1) {
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

void Level::addRoom(Room* room) {
  if (!this->currentRoom) this->currentRoom = room;
  this->rooms.push_back(room);
}

void Level::setRoomCount(int newRoomCount) {
  this->roomCount = newRoomCount;
}

void Level::generateFloor(SDL_Renderer* renderer) {
  // Generate at least 3 rooms, and at most 6
  int newRoomCount = rand_between(MIN_ROOM_COUNT, MAX_ROOM_COUNT);
  this->roomCount = newRoomCount;
  for (int room = 0; room < this->roomCount; ++room) {
    Room* newRoom = new Room;
    newRoom->generate(renderer);
    this->addRoom(newRoom);
  }
  if (!this->connectRooms()) {
    std::cerr << "Error: Room connection failed" << std::endl;
  }
  if (!this->placeStaircase()) {
    std::cerr << "Error: Staircase placing failed" << std::endl;
  }
}

void Level::advance(SDL_Renderer* renderer) {
  this->roomCount = 0;
  this->currentRoom = nullptr;
  this->staircaseRoom = nullptr;
  this->staircaseX = -1;
  this->staircaseY = -1;
  this->rooms.clear();
  this->generateFloor(renderer);
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

bool Level::placeStaircase() {
  // pick a room randomly
  int staircaseRoomIndex = rand_between(0, rooms.size() - 1);
  this->staircaseRoom = this->rooms[staircaseRoomIndex];
  int roomWidth = this->staircaseRoom->getWidth();
  int roomHeight = this->staircaseRoom->getHeight();
  // to avoid placing staircases in front of doorways
  int prohibitedX = roomWidth / 2;
  int prohibitedY = roomHeight / 2;
  for (int x = 1; x < roomWidth - 1; ++x) {
    // ignore if the current tile is in front of a doorway
    if (x == prohibitedX) continue;
    bool placeStaircase;
    for (int y = 1; y < roomHeight - 1; ++y) {
      // ignore if the current tile is in front of a doorway
      if (y == prohibitedY) continue;
      // 30% chance this tile gets a staircase
      placeStaircase = (rand_between(0, 100) <= 30) ? true : false;
      // place a staircase if the tile was chosen
      // OR if it's the last tile in the room
      if (placeStaircase || (x == roomWidth - 2 && y == roomHeight - 2)) {
        this->staircaseRoom->placeStaircase(x, y);
        this->staircaseX = x;
        this->staircaseY = y;
        placeStaircase = true;  // in case it's the last tile
        break;
      }
    }
    if (placeStaircase) return true;  // staircase placed, stop looking
  }
  return false;
}

std::pair<int, int> Level::getStaircasePosition() {
  return {this->staircaseX, this->staircaseY};
}
