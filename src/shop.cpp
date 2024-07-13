#include "shop.h"

Item::Item(PlayerAttributes attr, std::string name, int cost) {
  player_affect_ = attr;
  name_ = name;
  cost_ = cost;
}

Item::Item(const std::filesystem::path& path_to_set_up_file, const std::string& item_name) {
  std::ifstream data_file(path_to_set_up_file);
  Json::Value json_data;
  data_file >> json_data;
  data_file.close();
  for (auto item_information : json_data) {
    std::string potential_name = item_information["name_"].asString();
    if (item_name == potential_name) {
      name_ = potential_name;
      PlayerAttributes new_player_affect_(item_information["player_affect_"]);
      cost_ = item_information["cost_"].asInt();
      player_affect_ = new_player_affect_;
    }
  }
}

Inventory::Inventory(std::vector<Item> inv, std::map<std::string, int> count, int money, Player* owner) {
  inventory_ = inv;
  counts_ = count;
  money_ = money;
  owner_ = owner;
}

Inventory::Inventory(const std::filesystem::path& path_to_set_up_file, Player* owner) {
  std::ifstream data_file(path_to_set_up_file);
  Json::Value json_data;
  data_file >> json_data;
  money_ = START_MONEY;
  owner_ = owner;
  for (auto inventory_information : json_data) {
    std::string name = inventory_information["name_"].asString();
    PlayerAttributes player_affect(inventory_information["player_affect_"]);

    int cost = inventory_information["cost_"].asInt();
    int count = inventory_information["count_"].asInt();
    Item item(player_affect, name, cost);
    inventory_.push_back(item);
    counts_.insert({name, count});
  }

}

void Item::applyToPerson(Player& person) {
  person.changeAttributesWithoutRoom(player_affect_);
  person.getInventory().deleteItem(name_); 
}

bool Item::isEnoughMoney(Player& person) const { return (person.getInventory().getMoney() >= cost_); }

PlayerAttributes Item::getPlayerAffect() const { return player_affect_;}

std::string Item::getName() const { return name_; }

int Item::getCost() const { return cost_; }

Item Inventory::getItem(std::string name) {
  for (auto it : inventory_) {
    if (it.getName() == name) {
      return it;
    }
  }
  return Item();
}

int Inventory::getMoney() const { return money_; }

void Inventory::changeMoney(int delta) { money_ += delta; }

void Inventory::addItem(Item item) {
  for (auto it : inventory_) {
    if (it.getName() == item.getName()) {
      counts_[it.getName()] += 1;
      return;
    }
    inventory_.push_back(item);
    counts_.insert({item.getName(), 1});
  }
}

void Inventory::deleteItem(std::string name) {
  for (auto it : inventory_) {
    if (it.getName() == name && counts_[name] > 0) {
      counts_[it.getName()] -= 1;
    }
  }
}

std::vector<Item> Inventory::getCurrentInvetory() const { return inventory_; }

std::map<std::string, int> Inventory::getCounts() const { return counts_; }

bool Inventory::isItemContain(std::string name) const {
  for (auto p : counts_) {
    if (p.first == name && p.second > 0) {
      return true;
    }
  }
  return false;
}

Shop::Shop(const std::filesystem::path& path_to_set_up_file) : is_sale_(true) {
  std::ifstream data_file(path_to_set_up_file);
  Json::Value json_data;
  data_file >> json_data;
  data_file.close();
  for (auto shop_inforamtion : json_data) {
    std::string name = shop_inforamtion["name_"].asString();
    PlayerAttributes player_affect(shop_inforamtion["player_affect_"]);
    int cost = shop_inforamtion["cost_"].asInt();
    int count = shop_inforamtion["count_"].asInt();
    Item item(player_affect, name, cost);
    list_items_.push_back(item);
    counts_.insert({name, count});
  }
}

std::vector<Item> Shop::getCurrentItems() const { return list_items_; }

std::map<std::string, int> Shop::getCounts() const { return counts_; }

Item Shop::getItem(std::string name) {
  for (auto it : list_items_) {
    if (it.getName() == name) {
      return it;
    }
  }
  return Item("../data/items.json", "nothing");
}

void Shop::buyItem(std::string name, Player& person) {
  if (!isItemContain(name)) {
    return;
  }
  Item item = getItem(name);
  if (item.isEnoughMoney(person) && counts_[name] > 0) {
    Item item = getItem(name);
    person.getInventory().addItem(item);
    person.getInventory().changeMoney(-item.getCost());
    counts_[item.getName()] -= 1;
  }
}

void Shop::sellItem(std::string name, Player& person) {
  if (!person.getInventory().isItemContain(name)) {
    return;
  }
  Item item = getItem(name);
  person.getInventory().deleteItem(name);
  person.getInventory().changeMoney(item.getCost());
  counts_[item.getName()] += 1;
}

bool Shop::isItemContain(std::string name) const {
  for (auto p : counts_) {
    if (p.first == name && p.second > 0) {
      return true;
    }
  }
  return false;
}

void Shop::refresh(const Time& time) {
  if (time.day % WEEK_DURATION == 0) {
    is_sale_ = true;
    Shop new_shop("../data/shop.json");
    *this = new_shop;
  } else {
    is_sale_ = false;
  }
}

bool Shop::isSale() const { return is_sale_; }