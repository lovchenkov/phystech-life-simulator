#pragma once
#include "wrappers.h"

class Inventory;

class Player {
 private:
  PlayerAttributes attributes_;
  Room room_;
  Inventory* inventory_;

 public:
  Player(PlayerAttributes attr, Room room, Inventory* inv);
  PlayerAttributes getAttributes() const;
  void changeAttributes(PlayerAttributes attr);
  void changeAttributesWithoutRoom(PlayerAttributes attr);
  const Inventory& getInventory() const;
  Inventory& getInventory();
  void changeLocation(std::string new_room);
  void setInventory(Inventory* inventory);
  Room getRoom() const;
  bool isAlive() const;
};