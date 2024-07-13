#pragma once
#include "wrappers.h"
#include "action.h"
#include <vector>
#include <filesystem>
#include <fstream>

class TypeOfAction {
 protected:
  std::vector<Action*> actions_;

 public:
  TypeOfAction() = default;
  std::vector<Action*> getAvailableActions(Time time);
  std::vector<Action*> getActions() const;
};

class Subject : public TypeOfAction {
 private:
  Progress* progress_;

 public:
  Subject(const std::filesystem::path& path_to_set_up_file, const std::string& subject_name);
  Progress* getProgress() const;
};

class Chill : public TypeOfAction {
 public:
    Chill(const std::filesystem::path& path_to_set_up_file);
};

class Job : public TypeOfAction {
 public:
  Job(const std::filesystem::path& path_to_set_up_file);
};

bool timeComp(std::pair<Time, Action*> p1, std::pair<Time, Action*>p2);