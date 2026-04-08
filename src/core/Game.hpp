#pragma once

#include <SDL2/SDL.h>

#include <memory>
#include <vector>

#include "SDL2/SDL_render.h"
#include "SDL2/SDL_video.h"
#include "core/GameState.hpp"
#include "entities/Barrel.hpp"
#include "entities/DonkeyKong.hpp"
#include "entities/FireEnemy.hpp"
#include "entities/Mario.hpp"
#include "entities/Pauline.hpp"
#include "input/InputHandler.hpp"
#include "levels/Level.hpp"
#include "ui/ScoreManager.hpp"
#include "ui/UI.hpp"

class Game
{
 public:
  Game();
  ~Game();

  bool init();
  void run();

 private:
  // SDL context
  SDL_Window*   window_ = nullptr;
  SDL_Renderer* renderer_ = nullptr;

  // Core sys
  InputHandler input_;
  ScoreManager score_{"scores/highscore.txt"};
  UI*          ui_ = nullptr;
  GameState    state_ = GameState::MENU;
  bool         running_ = true;

  // level mgmnt
  int                    current_level_ = 0;
  std::unique_ptr<Level> level_;

  // entities
  std::unique_ptr<Mario>      mario_;
  std::unique_ptr<DonkeyKong> dk_;
  std::unique_ptr<Pauline>    pauline_;
  std::vector<Barrel>         barrels_;

  // timers
  float state_timer_ = 0.0f;
  float escape_timer_ = 0.0f;

  // game loop
  void fixedUpdate(float dt);
  void render();

  // state updaters
  void updateMenu();
  void updatePlaying(float dt);
  void updateEscaping(float dt);
  void updateLevelComplete(float dt);
  void updateGameOver();
  void updateWin();

  // helpers
  void loadLevel(int idx);
  void resetEntities();
  void checkCollisions();
  void spawnBarrelIfReady();

  static constexpr int LEVEL_NUMS[3] = {25, 50, 100};
};