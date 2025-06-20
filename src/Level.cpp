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
    void* adjacentRooms[4] = {
      room->getAdjacentRoom(NORTH),
      room->getAdjacentRoom(SOUTH),
      room->getAdjacentRoom(WEST),
      room->getAdjacentRoom(EAST)
    };
    if (_hasAnyConnection(adjacentRooms, 4)) roomConnected[i] = true;
  }
  // connect rooms without connections
  bool changeMade;
  bool roomsRemoved = false;
  // store all directions to try and connect randomly
  std::vector<Direction> directions = {NORTH, SOUTH, WEST, EAST};
  do {
    changeMade = false;
    // try to connect each room at least once
    for (size_t roomAIndex = 0;
    roomAIndex < this->rooms.size(); ++roomAIndex) {
      if (roomConnected[roomAIndex]) continue;  // ignore rooms w/ connections
      // try to connect to another room
      if (tryConnectRooms(roomAIndex, roomConnected, directions))
        changeMade = true;
      else {
        // room not connected, delete it
        this->rooms.erase(this->rooms.begin() + roomAIndex);
        roomConnected.erase(roomConnected.begin() + roomAIndex);
        --roomAIndex;
        roomsRemoved = true;
        changeMade = true;
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

bool Level::tryConnectRooms(size_t roomAIndex,
                            std::vector<bool>& roomConnected,
                            std::vector<Direction>& directions) {
  Room* roomA = rooms[roomAIndex];
  bool roomAConnected = false;
  // shuffle directions
  shuffleDirectionArray(directions);
  // try each direction
  for (Direction dir : directions) {
    // don't connect on a direction with an existing connection
    if (roomA->getAdjacentRoom(dir)) continue;
    // find a room to connect to
    for (size_t roomBIndex = 0;
    roomBIndex < rooms.size() && !roomAConnected; ++roomBIndex) {
      if (roomAIndex == roomBIndex) continue;  // don't connect with itself
      Room* roomB = rooms[roomBIndex];
      // get opposite direction
      Direction oppDir = static_cast<Direction>(_getOppositeDirection(dir));
      // skip if room B has a connection in the opposite direction
      if (roomB->getAdjacentRoom(oppDir) != nullptr) continue;
      // connect the rooms
      roomA->connect(dir, roomB);
      roomB->connect(oppDir, roomA);
      // create doorways
      roomA->createDoorway(dir);
      roomB->createDoorway(oppDir);
      // update flags
      roomAConnected = true;
      roomConnected[roomAIndex] = true;
      roomConnected[roomBIndex] = true;
    }
    if (roomAConnected) break;
  }
  return roomAConnected;
}

void Level::shuffleDirectionArray(std::vector<Direction>& directions) {
  static unsigned int seed = _seed();
  _shuffleDirections(reinterpret_cast<int*>(directions.data()),
                     directions.size(), seed);
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
  for (int x = 1; x < roomWidth - 1; ++x) {
    for (int y = 1; y < roomHeight - 1; ++y) {
      // validate staircase position to avoid placing in front of doorways
      if (!isValidStaircasePosition(x, y, roomWidth, roomHeight)) {
        continue;  // not a valid position, keep looking
      }
      // avoid placing the staircase too close to a doorway
      if (isDoorwayNearby(x, y)) {
        continue;
      }
      // 30% chance this tile gets a staircase
      bool placeStaircase = (rand_between(0, 100) <= 30) ? true : false;
      // place a staircase if the tile was chosen
      // OR if it's the last tile in the room
      if (placeStaircase || (x == roomWidth - 2 && y == roomHeight - 2)) {
        if (tryPlaceStaircase(x, y)) {
          return true;
        }
      }
    }
  }
  // if the staircase wasn't placed, try to place it in a corner of the room
  auto [x, y] = this->chooseCorner();
  return this->tryPlaceStaircase(x, y);
}

bool Level::isValidStaircasePosition(int x, int y, int width, int height) {
  // skip if the current tile is in front of a doorway
  int doorwayX = width / 2;
  int doorwayY = height / 2;
  if (x == doorwayX || y == doorwayY) return false;
  // validate position with asm helper
  return _checkValidStaircasePosition(x, y, width, height);
}

bool Level::isDoorwayNearby(int x, int y) {
  if (!staircaseRoom) return true;
  int roomWidth = staircaseRoom->getWidth();
  int roomHeight = staircaseRoom->getHeight();
  int doorwayX = roomWidth / 2;
  int doorwayY = roomHeight / 2;
  // check doorway proximity with asm helper
  return _isDoorwayProximity(x, y, doorwayX, doorwayY,
                             DOORWAY_PROXIMITY_THRESHOLD);
}

bool Level::tryPlaceStaircase(int x, int y) {
  if (!staircaseRoom) return false;
  this->staircaseRoom->placeStaircase(x, y);
  this->staircaseX = x;
  this->staircaseY = y;
  return true;
}

std::pair<int, int> Level::chooseCorner() {
  int corner = rand_between(0, 3);
  switch (corner) {
    case 0:  // top left
      return {2, 2};
    case 1:  // top right
      return {this->staircaseRoom->getWidth() - 2, 2};
    case 2:  // bottom left
      return {2, this->staircaseRoom->getHeight() - 2};
    case 3:  // bottom right
      return {this->staircaseRoom->getWidth() - 2,
        this->staircaseRoom->getHeight() - 2};
  }
  // Fallback return to silence compiler warning
  return {2, 2};
}

std::pair<int, int> Level::getStaircasePosition() {
  return {this->staircaseX, this->staircaseY};
}
