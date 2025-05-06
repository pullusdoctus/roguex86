#pragma once

#include <Room.hpp>

class Level {
  private:
    std::vector<Room*> rooms;
    int roomCount;
    int currentRoom;

  public:
    Level();
    ~Level();

    Room getRoom(int room);
    void addRoom(Room& room);

    void setRoomCount(int newRoomCount);
    int getRoomCount();
};
