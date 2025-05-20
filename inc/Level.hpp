#pragma once

#include <Room.hpp>

class Level {
  private:
    std::vector<Room*> rooms;
    int roomCount;
    Room* currentRoom;
    Room* staircaseRoom;
    int staircaseX;
    int staircaseY;

  public:
    Level();
    ~Level();

    Room getRoom(int room);
    Room* getCurrentRoom() { return this->currentRoom; }
    void addRoom(Room* room);

    void setRoomCount(int newRoomCount);
    int getRoomCount() { return this->roomCount; }

    void generateFloor(SDL_Renderer* renderer);
    void advance(SDL_Renderer* renderer);

    bool connectRooms();
    bool roomHasConnection(int i);
    void moveRoom(Room* nextRoom);

    bool placeStaircase();
    Room* getStaircaseRoom() { return this->staircaseRoom; }
    bool isValidStaircasePosition(int x, int y, int width, int height);
    bool isDoorwayNearby(int x, int y);
    bool tryPlaceStaircase(int x, int y);
    std::pair<int, int> getStaircasePosition();
};
