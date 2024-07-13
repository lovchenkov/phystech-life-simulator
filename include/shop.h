#pragma once
#include "wrappers.h"
#include "constants.h"
#include "player.h"
#include <map>
#include <string>
#include <fstream>
#include <filesystem>

class Item {
 private:
  PlayerAttributes player_affect_;
  std::string name_;
  int cost_;

 public:
  Item() = default;
  Item(PlayerAttributes attr, std::string name, int cost);
  Item(const std::filesystem::path& path_to_set_up_file, const std::string& item_name);
  Item& operator=(const Item& other) = default;
  void applyToPerson(Player& person);
  bool isEnoughMoney(Player& person) const;
  PlayerAttributes getPlayerAffect() const;
  std::string getName() const;
  int getCost() const;
};

class Inventory {
 private:
  std::vector<Item> inventory_;
  std::map<std::string, int> counts_;// (name, count)
  int money_;
  Player* owner_;

 public:
  Inventory(std::vector<Item> inv, std::map<std::string, int> count, int money, Player* owner);
  Inventory(const std::filesystem::path& path_to_set_up_file, Player* owner);
  Item getItem(std::string name);
  int getMoney() const;
  void changeMoney(int delta); // money_ += delta
  void addItem(Item item);
  void deleteItem(std::string name);
  std::vector<Item> getCurrentInvetory() const;
  std::map<std::string, int> getCounts() const;
  bool isItemContain(std::string name) const;
};


class Shop {
 private:
  std::vector<Item> list_items_;
  std::map<std::string, int> counts_; // (item, count)
  bool is_sale_;

 public:
  Shop(const std::filesystem::path& path_to_set_up_file);
  std::vector<Item> getCurrentItems() const; 
  std::map<std::string, int> getCounts() const;
  Item getItem(std::string name);
  bool isItemContain(std::string name) const;
  void buyItem(std::string name, Player& person); // person buy item
  void sellItem(std::string name, Player& person); // person sell item
  void refresh(const Time& time);
  bool isSale() const;
};
