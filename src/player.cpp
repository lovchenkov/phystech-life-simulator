#include "player.h"

Player::Player(PlayerAttributes attr, Room room, Inventory* inv) {
  attributes_ = attr;
  room_ = room;
  inventory_ = inv;
}

void Player::changeAttributes(PlayerAttributes attr) {
  attributes_ += attr * room_.coefficients;
}

void Player::changeAttributesWithoutRoom(PlayerAttributes attr) {
  attributes_ += attr;
}

PlayerAttributes Player::getAttributes() const { return attributes_; }

Inventory& Player::getInventory() {return *inventory_; }

const Inventory& Player::getInventory() const {return *inventory_; }

void Player::changeLocation(std::string new_room) {
  Room new_player_room("../data/room.json", new_room);
  room_ = new_player_room;
}

void Player::setInventory(Inventory* inventory) { inventory_ = inventory; }

Room Player::getRoom() const { return room_; }

bool Player::isAlive() const { return isValid(attributes_); }