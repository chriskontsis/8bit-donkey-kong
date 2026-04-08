
#include "ui/ScoreManager.hpp"

#include <fstream>

#include "core/Constants.hpp"

ScoreManager::ScoreManager(std::string_view save_path) : save_path_(save_path)
{
  reset();
  load();
}

ScoreManager::~ScoreManager()
{
  save();
}

void ScoreManager::addPoints(int pts)
{
  score += pts;
  if (score > high_score)
    high_score = score;
}

void ScoreManager::loseLife()
{
  --lives;
}

void ScoreManager::reset()
{
  score = 0;
  lives = Constants::MARIO_START_LIVES;
}

void ScoreManager::load()
{
  std::ifstream f(save_path_);
  if (f.is_open())
    f >> high_score;
}

void ScoreManager::save()
{
  std::ofstream f(save_path_);
  if (f.is_open())
    f << high_score;
}