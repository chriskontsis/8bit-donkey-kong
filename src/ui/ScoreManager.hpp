#pragma once
#include <string>
#include <string_view>

class ScoreManager
{
 public:
  int score = 0;
  int lives = 0;
  int high_score = 0;

  explicit ScoreManager(std::string_view save_path);
  ~ScoreManager();

  void addPoints(int pts);
  void loseLife();
  void reset();
  bool isGameOver() const { return lives <= 0; }

 private:
  std::string save_path_;
  void load();
  void save();
};