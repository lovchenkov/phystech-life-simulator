#include "shop.h"
#include "action.h"
#include "constants.h"
#include <iostream>

Action::Action(const std::string& action_name, const PlayerAttributes& player_affect, bool is_study,
               bool is_always_available, const Schedule& schedule)
    : action_name_(action_name), player_affect_(player_affect),
      is_study(is_study), is_always_available_(is_always_available), schedule_(schedule)  {}

StudyAction::StudyAction(const std::string& action_name,
    const PlayerAttributes& player_affect, bool is_study, bool is_always_available, Progress* subject,
    const Progress& additional_progress, const StudyRequirements& study_req,
    const StudyRequirements& req_change, const Schedule& schedule)
    : Action(action_name, player_affect, is_study, is_always_available, schedule), subject_(subject),
      additional_progress_(additional_progress),
      study_req_(study_req), req_change_(req_change) {}

ChillAction::ChillAction(const std::string &action_name, const PlayerAttributes &player_affect, bool is_study,
                         bool is_always_available, const Schedule &schedule) :
        Action(action_name, player_affect, is_study, is_always_available, schedule) {}

bool Action::isEnoughAttributes(const Player& player, int time) const {
  PlayerAttributes sum = player.getAttributes() + player_affect_ * time;
  return isValid(sum);
}

void Action::applyToPlayer(Player& player, int minutes) const {  // considers that it's applicable
  player.changeAttributes(player_affect_ * minutes);
}

bool StudyAction::isEnoughKnowledge(Time time) const {
  int prev_actions_num = 0;
  prev_actions_num += getSchedule().times.size() * (time.day / WEEK_DURATION);
  for (auto act_time : getSchedule().times) {
    if (act_time.first.day < time.day) {
      ++prev_actions_num;
    }
    if (act_time.first.isUpcoming(time)) {
      ++prev_actions_num;
    }
  }
  // std::cout << "\n##\n" << prev_actions_num << "\n##\n";
  return (getSubject()->practice >= getStudyRequirements().practice_req +
                                    prev_actions_num * getReqChange().practice_req &&
          getSubject()->theory >= getStudyRequirements().theory_req +
                                  prev_actions_num * getReqChange().theory_req);
}

void StudyAction::applyToSubject(int minutes) {  // considers that it's applicable
  *subject_ += getAddProgress() * minutes;
  study_req_ += req_change_;
}

PlayerAttributes Action::getPlayerAffect() const { return player_affect_; }

Schedule Action::getSchedule() const {
  return schedule_;
}

Progress* StudyAction::getSubject() const { return subject_; }

StudyRequirements StudyAction::getStudyRequirements() const { return study_req_; }

StudyRequirements StudyAction::getReqChange() const { return req_change_; }
Progress StudyAction::getAddProgress() const { return additional_progress_; }

bool Action::isStudy() const { return is_study; }

std::string Action::getName() const { return action_name_; }

int StudyAction::getDuration(Time time) const {
  for (auto act_time : schedule_.times) {
    if (time.isUpcoming(act_time.first)) {
      return act_time.second;
    }
  }
  return 0;
}

void StudyAction::applyAddProgress() { study_req_ += req_change_; }

Time StudyAction::getBeginningTime(Time time) const {
  for (auto act_time : schedule_.times) {
    if (time.isUpcoming(act_time.first)) {
      return act_time.first;
    }
  }
  return time;
}

bool Action::isAlwaysAvailable() const {
  return is_always_available_;
}

bool Action::isAvailable(Time time) const {
  bool result = false;
  if (is_always_available_) {
    result = true;
  }
  for (auto action_time : schedule_.times) {
    if (time.isUpcoming(action_time.first)) {
      result = true;
    }
  }
  return result;
}

int JobAction::getSalary() const { return salary_; }

void JobAction::applyToPlayer(Player& player, int minutes) const {  // considers that it's applicable
  player.changeAttributes(player_affect_ * minutes);
  player.getInventory().changeMoney(salary_ * minutes);
}

JobAction::JobAction(const std::string &action_name, const PlayerAttributes &player_affect, bool is_study,
                         bool is_always_available, const Schedule &schedule, int salary) :
        ChillAction(action_name, player_affect, is_study, is_always_available, schedule),
        salary_(salary) {}


int ChillAction::getDuration(Time time) const {
  return -1;
}
Time ChillAction::getBeginningTime(Time time) const {
  return {-1, -1};
}
Progress* ChillAction::getSubject() const {
  Progress* res = new Progress(-1, -1, -1, "hah");
  return res;
}

StudyRequirements ChillAction::getStudyRequirements() const {
  return StudyRequirements();
}

Progress ChillAction::getAddProgress() const {
  return Progress();
}
StudyRequirements ChillAction::getReqChange() const {
  return StudyRequirements();
}

bool ChillAction::isEnoughKnowledge(Time time) const {
  return false;
}

void ChillAction::applyAddProgress() {
  return;
}

void ChillAction::applyToSubject(int minutes) {
  return;
};

bool Action::isJob() const { return false; }
bool JobAction::isJob() const { return true; }
