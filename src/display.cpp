#include "display.h"
#include <iostream>

void Display::displayProgress(const Progress& progress) const {
  std::cout << "theory: " << progress.theory << '\n'
            << "practice: " << progress.practice << '\n'
            << "grade: " << progress.grade << '\n';
}

void Display::displayDayStudy(const Day& day) const {
  auto available_actions = day.getActions();
  int study_cnt = 0;
  for (int i = 0; i < available_actions.size(); ++i) {
    auto action = available_actions[i];
    if (action->isStudy() && !action->isAlwaysAvailable()) {
      ++study_cnt;
      std::cout << "- - - - - - - - - - - - - - - - - -\n";
      std::cout << i + 1 << " : ";
      displayAction(action, day.getTime());
    }
  }
  if (study_cnt == 0) {
    std::cout << "It's done for today, go to sleep\n";
  }
}

void Display::displayDayHomework(const Day& day) const {
  auto available_actions = day.getActions();
  if (available_actions.size() == 0) {
    std::cout << "It's done for today, go to sleep\n";
  }

  for (int i = 0; i < available_actions.size(); ++i) {
    auto action = available_actions[i];
    if (action->isStudy() && action->isAlwaysAvailable()) {
      std::cout << "- - - - - - - - - - - - - - - - - -\n";
      std::cout << i + 1 << " : ";
      displayAction(action, day.getTime());
    }
  }
}

void Display::displayDayChillWork(const Day& day) const {
  auto available_actions = day.getActions();
  if (available_actions.size() == 0) {
    std::cout << "It's done for today, go to sleep\n";
  }

  for (int i = 0; i < available_actions.size(); ++i) {
    auto action = available_actions[i];
    if (!action->isStudy()) {
      std::cout << "- - - - - - - - - - - - - - - - - -\n";
      std::cout << i + 1 << " : ";
      displayAction(action, day.getTime());
    }
  }
}

void Display::displayAttributes(const PlayerAttributes& player_attributes) const {
  std::cout << "energy: " << player_attributes.energy << '\n'
            << "loneliness: " << player_attributes.loneliness << '\n'
            << "ability to study: " << player_attributes.studying_capacity << '\n';
}

void Display::displayItemInformation(const Item& item) const {
  std::cout << "Name: " << item.getName() << '\n';
  std::cout << "Effects: ";
  displayAttributes(item.getPlayerAffect());
  std::cout << "Cost: " << item.getCost() << '\n';
}

void Display::displayInvetory(const Inventory& inventory) const {
  std::cout << "Current Invetory:" << '\n';
  std::cout << "Balance: " << inventory.getMoney() << '\n';
  for (auto& item : inventory.getCurrentInvetory()) {
    std::cout << item.getName() << " : " <<  inventory.getCounts()[item.getName()] << '\n';
  }
}

void Display::displayShop(const Shop& shop, const Player& player) const {
  std::cout << "Welcome to Hach's tent!!" << '\n';
  if (shop.isSale()) {
    std::cout << "Today is sale!! You can buy anything without limit!\n";
  }
  std::cout << "Your current balance: " << player.getInventory().getMoney() << '\n';
  for (auto& item : shop.getCurrentItems()) {
    std::cout << item.getName() << " : " << item.getCost() << " rubles. ";
    std::cout << "Left: " << shop.getCounts()[item.getName()] << std::endl;
    displayAttributes(item.getPlayerAffect());
    std::cout << '\n';
  }
}

void Display::displayAction(const Action* action, Time time) const {  // mb need to add beginnning
                                                           // time to chillAction
  if (action->isStudy()) {
    std::cout << action->getName() << " on " << action->getSubject()->name << '\n';
    if (!action->isAlwaysAvailable()) {
     std::cout << "Begins at " << action->getBeginningTime(time).getTime() << '\n';
     std::cout << "Lasts " << action->getDuration(time) << " minutes" << '\n';
    }
    std::cout << "Has following affect on you";
    if (action->isAlwaysAvailable()) {
      std::cout << " per minute";
    }
    std::cout << ":\n";
    PlayerAttributes player_affect;
    Progress subject_affect;
    if (!action->isAlwaysAvailable()) {
      player_affect = action->getPlayerAffect() * action->getDuration(time);
      subject_affect = action->getAddProgress() * action->getDuration(time);
    } else {
      player_affect = action->getPlayerAffect();
      subject_affect = action->getAddProgress();
    }
    displayAttributes(player_affect);
    std::cout << "Has following affect on subject";
    if (action->isAlwaysAvailable()) {
      std::cout << " per minute";
    }
    std::cout << ":\n";
    displayProgress(subject_affect);

  } else {
    std::cout << action->getName() << '\n'
            << "Has following affect on you";
    if (action->isAlwaysAvailable()) {
      std::cout << " per minute";
    }
    std::cout << ":\n";
    displayAttributes(action->getPlayerAffect());
  }
}

void Display::displayRoomCoefficients(const Room& room) const {
  std::cout << "Coefficients current room: ";
  displayAttributes(room.coefficients);
}

void Display::displayTime(const Time& time) const {  // 8:5 instead of 08:05
  std::cout << "You lived: " << time.day << " days\n";
  std::cout << "Current time is " << time.getTime() << '\n';
}

void Display::displayIntro() const {
  std::cout << 
  "You entered the Dolgoprudny town, the are only two ways\n"
  "to leave it -- to graduate MIPT or to be kicked out of it.\n"
  "It's only you to decide. You can sleep all day, or\n"
  "you can visit all lectures and seminars. To graduate, you\n"
  "should have at least 80 points of progress on each class.\n"
  "Good luck, warrior.\n\n";
}

void Display::displayHelp()  const {
  std::cout <<
  "see classes (SC) -- see all upcoming actions for today.\n\n"
  "see homework (SH) -- see all available homework.\n\n"
  "see chill and job (SCJ) -- see all available chills and jobs.\n\n"
  "To visit it, type the number that is next to the action.\n\n"
  "attributes (A) -- see your attributes.\n\n"
  "room (R) -- see your current room.\n\n"
  "change location (CL) -- change location"
  "time (T) -- see current time.\n\n"
  "progress (P) -- see progresses on each class.\n\n"
  "inventory (I) -- see your inventory.\n\n"
  "shop (S) -- see all goods in shop.\n\n"
  "buy item (BI) -- buy item.\n"
  "To buy it, you should enter the product's name.\n\n"
  "sell item (SI) -- sell item.\n"
  "To sell it, you should enter the product's name.\n\n"
  "apply item (AI) -- apply item.\n"
  "To apply it, you should enter the product's name.\n\n"
  "help (H) -- see this page again)\n\n"
  "end (E) -- end the game (not recommended))\n\n";
;
}

void Display::displayRoom(const Room& room) const {
  std::cout << "Now you are in " << room.name << '\n';
}