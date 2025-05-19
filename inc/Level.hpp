#pragma once

#include <Room.hpp>

class Level {
  private:
    std::vector<Room*> rooms;
    int roomCount;
    Room* currentRoom;

  public:
    Level();
    ~Level();

    Room getRoom(int room);
    Room* getCurrentRoom();
    void addRoom(Room* room);

    void setRoomCount(int newRoomCount);
    int getRoomCount();

    void generateFloor(SDL_Renderer* renderer);
    bool connectRooms();
    bool roomHasConnection(int i);
    void moveRoom(Room* nextRoom);
    bool placeStaircase();
};
