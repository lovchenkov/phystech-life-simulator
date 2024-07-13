#pragma once
#include <string>
#include <vector>
#include <fstream>
#include <filesystem>
#include <json/json.h>
struct PlayerAttributes {
  double energy;
  double loneliness;
  double studying_capacity;
  PlayerAttributes(int en, int lon, int st);
  PlayerAttributes& operator+=(PlayerAttributes attr);
  PlayerAttributes(Json::Value json_player_attributes);
  PlayerAttributes() = default;
  PlayerAttributes& operator*=(PlayerAttributes attr);
  PlayerAttributes& operator=(const PlayerAttributes&) = default;
  PlayerAttributes& operator*=(int a);
};

bool operator==(const PlayerAttributes& attr1, const PlayerAttributes& attr2);

PlayerAttributes operator+(PlayerAttributes attr1, PlayerAttributes attr2);

PlayerAttributes operator*(PlayerAttributes attr1, PlayerAttributes attr2);

bool isValid(const PlayerAttributes& attr);

struct Room {
  std::string name;
  PlayerAttributes coefficients;
  Room() = default;
  Room(const std::filesystem::path& path_to_set_up_file, const std::string& item_name);
};

struct Time {
  int day;
  int time;
  Time(int time);
  Time(int day, int time); 
  bool isUpcoming(Time other);  // checks if time is "in future" and today
  Time(Json::Value time);
  Time& operator+=(int duration);
  std::string getTime() const;
};

struct Schedule {
  std::vector<std::pair<Time, int>> times;  // pair keeps (time of begining, duration)
  Schedule(Json::Value json_schedule);
};

struct StudyRequirements {
  double theory_req;
  double practice_req;
  StudyRequirements& operator+=(const StudyRequirements& other);
  StudyRequirements() = default;
  StudyRequirements(Json::Value json_study_requirements);
};

struct Progress {
  double theory;
  double practice;
  double grade;
  std::string name;
  Progress() = default;
  Progress& operator+=(Progress progress);
  Progress& operator*=(int a);
  Progress(Json::Value json_progress);
  Progress(int th, int pr, int gr, const std::string& nm);
};

Progress operator*(Progress a, int b);
PlayerAttributes operator*(PlayerAttributes a, int b);