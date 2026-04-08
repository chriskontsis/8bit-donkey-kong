#pragma once

#include <SDL2/SDL.h>

#include <array>
#include <expected>
#include <memory>
#include <string>
#include <vector>

#include "SDL2/SDL_render.h"
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
  Game() = default;
  ~Game() { SDL_Quit(); }

  std::expected<void, std::string> init();
  void run();

 private:
  using WindowPtr = std::unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)>;
  using RendererPtr = std::unique_ptr<SDL_Renderer, decltype(&SDL_DestroyRenderer)>;

  WindowPtr           window_{nullptr, SDL_DestroyWindow};
  RendererPtr         renderer_{nullptr, SDL_DestroyRenderer};
  std::unique_ptr<UI> ui_;

  // Core sys
  InputHandler input_;
  ScoreManager score_{"scores/highscore.txt"};
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

  static constexpr std::array<int, 3> LEVEL_NUMS{25, 50, 100};
};