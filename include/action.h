#pragma once
#include "wrappers.h"
#include "player.h"
#include <string>

class Action {
 protected:
  std::string action_name_;
  PlayerAttributes player_affect_;
  bool is_study;
  bool is_always_available_;
  Schedule schedule_;
 public:
  PlayerAttributes getPlayerAffect() const;
  std::string getName() const;
  Action(const std::string& action_name, const PlayerAttributes& player_affect,
      bool is_study, bool is_always_available, const Schedule& schedule);
  bool isEnoughAttributes(const Player& player, int time) const;
  void applyToPlayer(Player& player, int minutes) const;
  virtual void applyToSubject(int minutes) = 0;
  Schedule getSchedule() const;
  virtual Progress* getSubject() const = 0;
  virtual StudyRequirements getStudyRequirements() const = 0;
  virtual Progress getAddProgress() const = 0;
  virtual StudyRequirements getReqChange() const = 0;
  virtual bool isEnoughKnowledge(Time time) const = 0;
  virtual void applyAddProgress() = 0;
  bool isStudy() const;
  virtual bool isJob() const;
  virtual int getDuration(Time time) const = 0;
  virtual Time getBeginningTime(Time time) const = 0;
  bool isAvailable(Time time) const;
  bool isAlwaysAvailable() const;
};


class ChillAction : public Action {
 public:
  ChillAction(const std::string& action_name, const PlayerAttributes& player_affect,
      bool is_study, bool is_always_available, const Schedule& schedule);
  int getDuration(Time time) const;
  Time getBeginningTime(Time time) const;
  Progress* getSubject() const;
  StudyRequirements getStudyRequirements() const;
  Progress getAddProgress() const;
  StudyRequirements getReqChange() const;
  bool isEnoughKnowledge(Time time) const;
  void applyAddProgress();
  void applyToSubject(int times);
};

class JobAction : public ChillAction {
 private:
  int salary_;  // per minute

 public:
   JobAction(const std::string& action_name, const PlayerAttributes& player_affect,
      bool is_study, bool is_always_available, const Schedule& schedule, int salary);

  int getSalary() const;
  void applyToPlayer(Player& player, int minutes) const;
  bool isJob() const override;
};

class StudyAction : public Action {
 private:
  Progress* subject_;
  Progress additional_progress_;
  StudyRequirements req_change_;
  StudyRequirements study_req_;

 public:
  bool isEnoughKnowledge(Time time) const override;
  void applyAddProgress() override;
  void applyToSubject(int minutes) override;
  Progress* getSubject() const override;
  StudyRequirements getStudyRequirements() const override;
  StudyRequirements getReqChange() const override;
  Progress getAddProgress() const override;
  int getDuration(Time time) const override;
  Time getBeginningTime(Time time) const override;
  StudyAction(const std::string& action_name,
      const PlayerAttributes& player_affect, bool is_study, bool is_always_available,
      Progress* subject, const Progress& additional_progress,
      const StudyRequirements& study_req, const StudyRequirements& req_change,
      const Schedule& schedule);
};