#pragma once
#include "shop.h"
#include "action.h"
#include "day.h"
#include "constants.h"

class Display{
 public:
  void displayDayStudy(const Day& day) const;  // before using don't forget to updateActions
  void displayDayHomework(const Day& day) const;
  void displayDayChillWork(const Day& day) const;
  void displayProgress(const Progress& progress) const;
  void displayAttributes(const PlayerAttributes& player_attributes) const;
  void displayItemInformation(const Item& item) const;
  void displayInvetory(const Inventory& inventory) const;
  void displayShop(const Shop& shop, const Player& player) const;
  void displayAction(const Action* action, Time time) const;
  void displayRoomCoefficients(const Room& room) const;
  void displayTime(const Time& time) const;
  void displayIntro() const;
  void displayHelp() const;
  void displayRoom(const Room& room) const;
};