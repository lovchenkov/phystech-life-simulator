#include "typeOfAction.h"
#include <algorithm>

Subject::Subject(const std::filesystem::path& path_to_set_up_file, const std::string& subject_name)
    : progress_(new Progress(0, 0, 0, subject_name)) {
  std::ifstream data_file(path_to_set_up_file);
  Json::Value json_data;
  data_file >> json_data;
  data_file.close();
  for (auto json_action : json_data) {
    std::string name = json_action["action_name_"].asString();
    PlayerAttributes player_affect(json_action["player_affect_"]);
    bool is_study = json_action["is_study"].asBool();
    bool is_always_available = json_action["is_always_available"].asBool();
    Schedule schedule(json_action["schedule_"]);
    Progress additional_progress(json_action["additional_progress_"]);
    StudyRequirements study_req(json_action["study_req_"]);
    StudyRequirements req_change(json_action["req_change_"]);
    Action *action = new StudyAction(name, player_affect, is_study, is_always_available, progress_,
                                     additional_progress, study_req, req_change,
                                     schedule);
    actions_.push_back(action);
  }
}

Chill::Chill(const std::filesystem::path &path_to_set_up_file) {
  std::ifstream data_file(path_to_set_up_file);
  Json::Value json_data;
  data_file >> json_data;
  data_file.close();
  for (auto json_action : json_data) {
    std::string name = json_action["action_name_"].asString();
    PlayerAttributes player_affect(json_action["player_affect_"]);
    bool is_study = json_action["is_study"].asBool();
    bool is_always_available = json_action["is_always_available"].asBool();
    Schedule schedule(json_action["schedule_"]);
    Action *action = new ChillAction(name, player_affect, is_study, is_always_available, schedule);
    actions_.push_back(action);
  }
}

Job::Job(const std::filesystem::path &path_to_set_up_file) {
  std::ifstream data_file(path_to_set_up_file);
  Json::Value json_data;
  data_file >> json_data;
  data_file.close();
  for (auto json_action : json_data) {
    std::string name = json_action["action_name_"].asString();
    PlayerAttributes player_affect(json_action["player_affect_"]);
    bool is_study = json_action["is_study"].asBool();
    bool is_always_available = json_action["is_always_available"].asBool();
    Schedule schedule(json_action["schedule_"]);
    int salary = json_action["salary_"].asInt();
    Action *action = new JobAction(name, player_affect, is_study, is_always_available, schedule, salary);
    actions_.push_back(action);
  }
}

std::vector<Action*> TypeOfAction::getActions() const {
  return actions_;
}


bool timeComp(std::pair<Time, Action*> p1, std::pair<Time, Action*> p2) {
  return p1.first.time < p2.first.time;
}

std::vector<Action*> TypeOfAction::getAvailableActions(Time time) {
  std::vector<Action*> available_actions;
  std::vector<std::pair<Time, Action*>> sorted;
  for (Action* action : getActions()) {
    if (action->isAvailable(time)) {
      available_actions.push_back(action);
    }
  }
  return available_actions;
}

Progress* Subject::getProgress() const { return actions_[0]->getSubject(); }