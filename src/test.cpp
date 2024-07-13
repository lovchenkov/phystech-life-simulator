#include <iostream>
#include "constants.h"
#include "wrappers.h"
#include "player.h"
#include "action.h"
#include "typeOfAction.h"
#include "display.h"

const int tab_size = 4;
#define print(depth, name, par) std::cout << GetSpace(depth) << name << ": " << par << std::endl;  
std::string GetSpace(int depth) {
  std::string res = "";
  for (int i = 0; i < tab_size * depth; ++i) {
    res += " ";
  }
  return res;
}

void PrintPlayerAttributes(const PlayerAttributes& attr, int depth) {
  print(depth, "energy", attr.energy);
  print(depth, "loneliness", attr.loneliness);
  print(depth, "studying_capacity", attr.studying_capacity);
}
void PrintProgress(const Progress& progress, int depth) {
  print(depth, "theory", progress.theory);
  print(depth, "practice", progress.practice);
  print(depth, "grade", progress.grade);
}

void PrintStudyRequirements(const StudyRequirements& req, int depth) {
  print(depth, "theory_req", req.theory_req);
  print(depth, "practice_req", req.practice_req);
}

void PrintTime(const std::pair<Time, int>& time, int depth) {
  print(depth, "day", time.first.day);
  print(depth, "time", time.first.time);
  print(depth, "duration", time.second)
}

void PrintSchedule(const Schedule& schedule, int depth) {
  for (size_t i = 0; i < schedule.times.size(); ++i) {
    auto time = schedule.times[i]; 
    std::cout << GetSpace(depth) << "time_" << i << ": " << std::endl;
    PrintTime(time, depth + 1);
  }
}

void PrintAction(Action* action, int depth) {
  print(depth, "action_name", action->getName());
  std::cout << GetSpace(depth) << "player_affect: " << std::endl;
  PrintPlayerAttributes(action->getPlayerAffect(), depth + 1);
  print(depth, "is_study", action->isStudy());
  if (action->isStudy()) {
    std::cout << GetSpace(depth) << "additional_progress: " << std::endl;
    PrintProgress(action->getAddProgress(), depth + 1);
    std::cout << GetSpace(depth) << "study_req: " << std::endl;
    PrintStudyRequirements(action->getStudyRequirements(), depth + 1);
    std::cout << GetSpace(depth) << "req_change: " << std::endl;
    PrintStudyRequirements(action->getReqChange(), depth + 1);
    std::cout << GetSpace(depth) << "schedule: " << std::endl;
    PrintSchedule(action->getSchedule(), depth + 1);
  }
}

void PrintTypeOfAction(const Subject& type) {
  for (auto action : type.getActions()) {
    PrintAction(action, 1);
  }
  std::cout << "-----------------------------------------\n";
}
// int main() {
//   std::vector<TypeOfAction*> all_types;
//   Subject calculus("../data/calculus.json", "calculus");
//   all_types.push_back(&calculus);
//   Display screen;
//   Time current_time(0);
//   Day day(current_time, all_types);
//   current_time = Time(7 * DAY_DURATION);
//   day.updateActions(current_time);
//   screen.displayDay(day);;
// }
