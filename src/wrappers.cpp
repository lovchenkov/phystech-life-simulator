#include "wrappers.h"
#include "constants.h"
PlayerAttributes::PlayerAttributes(Json::Value json_player_attrubutes) {
  energy = json_player_attrubutes["energy"].asDouble();
  loneliness = json_player_attrubutes["loneliness"].asDouble();
  studying_capacity = json_player_attrubutes["studying_capacity"].asDouble();
}

Time::Time(Json::Value json_time) {
  day = json_time["day"].asInt();
  time = json_time["time"].asInt();
}

Schedule::Schedule(Json::Value json_schedule) {
  for (Json::Value json_pair : json_schedule) {
    times.push_back({Time(json_pair["time"]), json_pair["duration"].asInt()});
  }
}

StudyRequirements::StudyRequirements(Json::Value json_study_requirements) {
  theory_req = json_study_requirements["theory_req"].asDouble();
  practice_req = json_study_requirements["practice_req"].asDouble();
}

Progress::Progress(Json::Value json_progress) {
  theory = json_progress["theory"].asDouble();
  practice = json_progress["practice"].asDouble();
  grade = json_progress["grade"].asDouble();
  name = json_progress["name"].asString();
}

Progress::Progress(int th, int pr, int gr, const std::string& nm)
      : theory(th), practice(pr), grade(gr), name(nm) {}

Time::Time(int time) : day(time / DAY_DURATION), time(time % DAY_DURATION) {}

Time::Time(int day, int time) : day(day), time(time) {}

bool Time::isUpcoming(Time other) {
  return (day % WEEK_DURATION == other.day % WEEK_DURATION && time <= other.time);
}


PlayerAttributes& PlayerAttributes::operator+=(PlayerAttributes attr) {
  energy += attr.energy;
  loneliness += attr.loneliness;
  studying_capacity += attr.studying_capacity;
  return *this;
}

PlayerAttributes operator+(PlayerAttributes attr1, PlayerAttributes attr2) {
  return (attr1 += attr2);
}

PlayerAttributes& PlayerAttributes::operator*=(PlayerAttributes attr) {
  energy *= attr.energy;
  loneliness *= attr.loneliness;
  studying_capacity *= attr.studying_capacity;
  return *this;
}

PlayerAttributes operator*(PlayerAttributes attr1, PlayerAttributes attr2) {
  return (attr1 *= attr2);
}

bool operator==(const PlayerAttributes& attr1, const PlayerAttributes& attr2) {
  return (attr1.energy == attr2.energy &&
          attr1.loneliness == attr2.loneliness &&
          attr1.studying_capacity == attr2.studying_capacity);
}

Room::Room(const std::filesystem::path& path_to_set_up_file, const std::string& room_name) {
  std::ifstream data_file(path_to_set_up_file);
  Json::Value json_data;
  data_file >> json_data;
  data_file.close();
  for (auto room_json : json_data) {
    std::string potential_name = room_json["name"].asString();
    if (potential_name == room_name) {
      PlayerAttributes new_coefficients(room_json["coefficients"]);
      coefficients = new_coefficients;
      name = potential_name;
    }
  }
}

Progress& Progress::operator+=(Progress progress) {
  theory += progress.theory;
  practice += progress.practice;
  return *this;
}

bool isValid(const PlayerAttributes& attr) {
  return (attr.energy > 0 &&
          attr.loneliness > 0 &&       // considering that when you're not lonely
          attr.studying_capacity > 0); // your loneliness is MAX_LONELINESS
}

StudyRequirements& StudyRequirements::operator+=(const StudyRequirements& other) {
  theory_req += other.theory_req;
  practice_req += other.practice_req;
  return *this;
}

PlayerAttributes::PlayerAttributes(int en, int lon, int st) :
                  energy(en),
                  loneliness(lon),
                  studying_capacity(st) {}

Time& Time::operator+=(int duration) {
  time += duration;
  if (time >= DAY_DURATION) {
    day += time / DAY_DURATION;
    time %= DAY_DURATION;
  }
  return *this;
}

std::string Time::getTime() const {
  int h = time / HOUR_DURATION;
  int m = time % HOUR_DURATION;
  return (h < 10 ? "0" + std::to_string(h) : std::to_string(h)) +
          ":" +
          (m < 10 ? "0" + std::to_string(m) : std::to_string(m));
}

Progress& Progress::operator*=(int a) {
    theory *= a;
    practice *= a;
    grade *= a;
    return *this;
}

Progress operator*(Progress a, int b) {
  return a *= b;
}

PlayerAttributes &PlayerAttributes::operator*=(int a) {
  energy *= a;
  loneliness *= a;
  studying_capacity *= a;
  return *this;
}

PlayerAttributes operator*(PlayerAttributes a, int b) {
  return a *= b;
}