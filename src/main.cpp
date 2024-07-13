#include "main.h"

bool isNumber(const std::string& s) {
    for (char const &ch : s) {
        if (std::isdigit(ch) == 0) 
            return false;
    }
    return true;
}

void addSubject(const std::filesystem::path& path_to_json, const std::string& name,
                std::map<std::string, Progress*>& progress, std::vector<TypeOfAction*>& all_activities) {
  auto subject = new Subject(path_to_json, name);
  all_activities.push_back(subject);
    progress[name] = subject->getProgress();
}

void setAvailableRooms(const std::filesystem::path& path_to_json, std::vector<std::string>& available_rooms) {
  std::ifstream data_file(path_to_json);
  Json::Value json_data;
  data_file >> json_data;
  data_file.close();
  for (auto room_json : json_data) {
    std::string new_room_name = room_json["name"].asString();
    available_rooms.push_back(new_room_name);
  }
}

bool isRoomAccessible(std::string room, const std::vector<std::string>& available_rooms) {
  for (auto new_room : available_rooms) {
    if (new_room == room) {
      return true;
    }
  }
  return false;
}

void addChill(const std::filesystem::path& path_to_json, std::vector<TypeOfAction*>& all_activities) {
  auto  chill = new Chill(path_to_json);
  all_activities.push_back(chill);
}

void addJob(const std::filesystem::path& path_to_json, std::vector<TypeOfAction*>& all_activities) {
  auto job = new Job(path_to_json);
  all_activities.push_back(job);
}

void game() {
  std::map<std::string, Progress*> progress;
  std::vector<TypeOfAction*> all_activities;

  // Initialising all TypeOfActions
  addSubject("../data/calculus.json", "Calculus", progress, all_activities);
  addSubject("../data/algem.json", "AlGem", progress, all_activities);
  addSubject("../data/mathlog.json", "MathLog", progress, all_activities);
  addSubject("../data/fcnt.json", "FCNT", progress, all_activities);
  addSubject("../data/algo.json", "Algo", progress, all_activities);
  addSubject("../data/tp.json", "TP", progress, all_activities);
  addSubject("../data/c++.json", "C++", progress, all_activities);
  addSubject("../data/python.json", "Python", progress, all_activities);
  addSubject("../data/english.json", "English", progress, all_activities);
  addChill("../data/chill.json", all_activities);
  addJob("../data/job.json", all_activities);

  // Initialising Day
  Day day(Time(0), all_activities);

  // Initialising Room
  Room common_room("../data/room.json", "room");
  std::vector<std::string> available_rooms;
  setAvailableRooms("../data/room.json", available_rooms);

  // Initialising Player
  Player player(PlayerAttributes(MAX_ENERGY, MAX_LONELINESS, MAX_STUDYING_CAPACITY),
                common_room,
                nullptr);

  Inventory inventory("../data/start_invetory.json", &player);

  player.setInventory(&inventory);

  Shop shop("../data/shop.json");

  Display display;
  display.displayIntro();
  display.displayHelp();

  std::string request = "";
  while (request != "exit" && request != "E") {
    if (!player.isAlive()) {
      std::cout << "YOU DIDN'T SURVIVE IN DOLGOPRUDNY TOWN\n";
      return;
    }

    std::cout << "__________________________________\n";
    std::getline(std::cin, request);

    shop.refresh(day.getTime());
    if (request == "attributes" || request == "A") {
      display.displayAttributes(player.getAttributes());
      continue;
    } 
    if (request == "see classes" || request == "SC") {
      day.updateActions();
      display.displayDayStudy(day);
      continue;
    }
    if (request == "see homework" || request == "SH") {
      day.updateActions();
      display.displayDayHomework(day);
      continue;
    }
    if (request == "see chill and job" || request == "SCJ") {
      day.updateActions();
      display.displayDayChillWork(day);
      continue;
    }
    if (isNumber(request)) {
      if (std::stoi(request) <= 0 || std::stoi(request) > day.getActions().size()) {  // in the future if can
                                                                                     // be changed in case of REAL TIME
        std::cout << "Enter correct index of action\n";
        continue;
      }
      int index = std::stoi(request) - 1;
      Action* action = day.getActionByIndex(index);
      int time;
      if (action->isAlwaysAvailable()) {
        std::cout << "Enter time of doing this action\n";
        std::string string_time;
        std::getline(std::cin, string_time);
        if (!isNumber(string_time) || std::stoi(string_time) <= 0) {
          std::cout << "Enter correct time of action\n";
          continue;
        }
        time = std::stoi(string_time);
      } else {
        time = action->getDuration(day.getTime());
      }
      if (!day.isEnoughAttributes(std::stoi(request) - 1, player, time)) {
        std::cout << "Go chill, man, you don't have enough attribiutes\n";
        continue;
      }
      if (action->isStudy() && !day.isEnoughKnowledge(std::stoi(request) - 1)) {
        std::cout << "Go study, man, you don't have enough knowledge\n";
        continue;
      }
      day.applyAction(index, player, time);
      if (action->isStudy()) {
        auto act_progress = progress[action->getSubject()->name];
        std::cout << "Your current progress on " << act_progress->name << ":\n";
        display.displayProgress(*act_progress);
      }
      continue;
    }
    if (request == "time" || request == "T") {
      display.displayTime(day.getTime());
      continue;
    }
    if (request == "skip") {
      day.setTime(Time(day.getTime().day + 1, 0));
      continue;
    }
    if (request == "progress" || request == "P") {
      for (auto p : progress) {
        std::cout << "- - - - - - - - - - - - - - - - - -\n";
        std::cout << p.first << '\n';
        display.displayProgress(*p.second);
      }
      continue;
    }
    if (request == "inventory" || request == "I") {
      display.displayInvetory(inventory);
      continue;
    }
    if (request == "shop" || request == "S") {
      display.displayShop(shop, player);
      continue;
    }
    if (request == "buy item" || request == "BI") {
      std::cout << "What do you want to buy?\n";
      std::string bought_item;
      std::getline(std::cin, bought_item);
      if (!shop.isItemContain(bought_item)) {
        std::cout << "Sorry, we don't have this. Come in other day.\n";
      } else if (!shop.getItem(bought_item).isEnoughMoney(player)) {
        std::cout << "Sorry, you don't have enough money. Earn more and come later.\n";
      } else {
        shop.buyItem(bought_item, player);
        std::cout << "Thank you for your purchase. Come again!\n";
      }
      continue;
    }
    if (request == "apply item" || request == "AI") {
      std::cout << "What item do you want to apply?\n";
      std::string apply;
      std::getline(std::cin, apply);
      if (!player.getInventory().isItemContain(apply)) {
        std::cout << "You don't have this item)))))\n";
      } else {
        Item item = player.getInventory().getItem(apply);
        item.applyToPerson(player);
        std::cout << "Your attributes changed by:\n";
        display.displayItemInformation(item);
      }
      continue;
    }
    if (request == "sell item" || request == "SI") {
      std::cout << "What do you want to sell?\n";
      std::string sell_item;
      std::getline(std::cin, sell_item);
      if (!player.getInventory().isItemContain(sell_item)) {
        std::cout << "You don't have this item)))))\n";
      } else {
        shop.sellItem(sell_item, player);
        std::cout << "Thanks for selling.\n";
      }
      continue;
    }
    if (request == "room" || request == "R") {
      display.displayRoom(player.getRoom());
      continue;
    }
    if (request == "change location" || request == "CL") {
      std::cout << "Where do you want to go?\n";
      std::string room;
      std::getline(std::cin, room);
      if (!isRoomAccessible(room, available_rooms)) {
        std::cout << "You don't know where it is...\n";
      } else {
        player.changeLocation(room);
        std::cout << "Now you are in " << room << '\n';
        std::cout << "Current room coeficient:\n";
        display.displayAttributes(player.getRoom().coefficients);
      }
      continue;
    }
    if (request == "help" || request == "H") {
      display.displayHelp();
      continue;
    }
  }
}

int main() {
  game();
}