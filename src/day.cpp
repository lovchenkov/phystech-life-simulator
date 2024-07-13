#include "day.h"
#include "algorithm"

void Day::updateActions() {
  actions_.clear();
  std::vector<Action*> all_actions;
  for (auto type_of_action : type_of_actions_) {
    std::vector<Action*> available_actions = type_of_action->getAvailableActions(time_);
    for (Action* action : available_actions) {
      all_actions.push_back(action);
    }
  }
  std::vector<Action*> always_available_actions;
  std::vector<std::pair<Time, Action*>> schedule_actions;
  for (auto action : all_actions) {
    if (action->isAlwaysAvailable()) {
      always_available_actions.push_back(action);
    } else {
      schedule_actions.push_back({action->getBeginningTime(time_), action});
    }
  }
  std::sort(schedule_actions.begin(), schedule_actions.end(), timeComp);
  for (auto action : schedule_actions) {
    actions_.push_back(action.second);
  }
  for (auto action : always_available_actions) {
    actions_.push_back(action);
  }
}

std::vector<Action*> Day::getActions() const { return actions_; }

Action* Day::getActionByIndex(int index) {
  return (actions_[index]);
}

void Day::applyAction(int index, Player& player, int minutes) {
  Action* action = actions_[index];
  if (action->isJob()) {
    static_cast<JobAction*>(action)->applyToPlayer(player, minutes);
  } else {
    action->applyToPlayer(player, minutes);
  }
  if (action->isStudy()) {
    action->applyToSubject(minutes);
  }
  if (!action->isAlwaysAvailable()) {
    time_.time = action->getBeginningTime(time_).time;
  }
  time_ += minutes;
}

bool Day::isEnoughAttributes(int index, const Player& player, int time) const {
  return actions_[index]->isEnoughAttributes(player, time);
}

bool Day::isEnoughKnowledge(int index) const {
  return actions_[index]->isEnoughKnowledge(time_);
}

Day::Day(Time time, std::vector<TypeOfAction*> type_of_actions) :
            time_(time),
            actions_(std::vector<Action*>(0)),
            type_of_actions_(type_of_actions) {}

Time Day::getTime() const { return time_; }

void Day::setTime(Time time) { time_ = time; }