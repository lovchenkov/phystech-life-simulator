#pragma once
#include "wrappers.h"
#include "player.h"
#include "action.h"
#include "typeOfAction.h"
#include <vector>

class Day {
 private:
  Time time_;
  std::vector<Action*> actions_;
  std::vector<TypeOfAction*> type_of_actions_;

 public:
  Day() = default;
  Day(Time time, std::vector<TypeOfAction*> type_of_actions);
  void updateActions();
  void applyAction(int index, Player& player, int minutes);
  bool isEnoughAttributes(int index, const Player& player, int time) const;
  bool isEnoughKnowledge(int index) const;
  std::vector<Action*> getActions() const;
  Time getTime() const;
  void setTime(Time time);
  Action* getActionByIndex(int index);
  int getAction(int index);
};