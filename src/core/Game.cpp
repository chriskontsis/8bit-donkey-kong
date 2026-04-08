#include "core/Game.hpp"

#include <SDL2/SDL_video.h>

#include <memory>

#include "SDL2/SDL.h"
#include "SDL2/SDL_render.h"
#include "SDL2/SDL_scancode.h"
#include "SDL2/SDL_timer.h"
#include "SDL2/SDL_video.h"
#include "core/Constants.hpp"
#include "core/GameState.hpp"
#include "levels/Level100m.hpp"
#include "levels/Level25m.hpp"
#include "levels/Level50m.hpp"

// Construction / destruction
Game::Game() {}
Game::~Game()
{
  delete ui_;
  if (renderer_)
    SDL_DestroyRenderer(renderer_);
  if (window_)
    SDL_DestroyWindow(window_);
  SDL_Quit();
}

// Init
bool Game::init()
{
  if (SDL_Init(SDL_INIT_VIDEO) != 0)
    return false;

  window_ = SDL_CreateWindow("Donkey Kong", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                             Constants::WINDOW_WIDTH, Constants::WINDOW_HEIGHT, SDL_WINDOW_SHOWN);

  if (!window_)
    return false;

  renderer_ = SDL_CreateRenderer(window_, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  if (!renderer_)
    return false;

  ui_ = new UI(renderer_);
  return true;
}

// Main loop
void Game::run()
{
  Uint64 prev = SDL_GetPerformanceCounter();
  float  freq = static_cast<float>(SDL_GetPerformanceFrequency());
  float  accum = 0.0f;

  while (running_)
  {
    Uint64 now = SDL_GetPerformanceCounter();
    float  dt = static_cast<float>(now - prev) / freq;
    prev = now;

    if (dt > Constants::MAX_ACCUM)
      dt = Constants::MAX_ACCUM;
    accum += dt;

    input_.update();
    if (input_.quitRequested())
      running_ = false;

    while (accum >= Constants::FIXED_DT)
    {
      fixedUpdate(Constants::FIXED_DT);
      input_.clearPressed();
      accum -= Constants::FIXED_DT;
    }

    render();
  }
}

// Fixed Update - dispatch by state
void Game::fixedUpdate(float dt)
{
  switch (state_)
  {
    case GameState::MENU:
      updateMenu();
      break;
    case GameState::PLAYING:
      updatePlaying(dt);
      break;
    case GameState::DK_ESCAPING:
      updateEscaping(dt);
      break;
    case GameState::LEVEL_COMPLETE:
      updateLevelComplete(dt);
      break;
    case GameState::GAME_OVER:
      updateGameOver();
      break;
    case GameState::WIN:
      updateWin();
      break;
  }
}

// ── State: MENU
void Game::updateMenu()
{
  if (input_.justPressed(SDL_SCANCODE_RETURN))
  {
    current_level_ = 0;
    score_.reset();
    loadLevel((current_level_));
    state_ = GameState::PLAYING;
  }
}

// ── State: PLAYING
void Game::updatePlaying(float dt)
{
  if (!mario_ || !dk_ || !level_)
    return;

  // update entites
  mario_->update(dt, input_, level_->platforms, level_->ladders);
  dk_->update(dt);
  pauline_->update(dt);

  for (auto& b : barrels_)
    b.update(dt, level_->platforms, level_->ladders);
  for (auto& f : level_->fire_enemies)
    f.update(dt);

  // remove inactive barrels
  barrels_.erase(
      std::remove_if(barrels_.begin(), barrels_.end(), [](const Barrel& b) { return !b.active; }),
      barrels_.end());

  spawnBarrelIfReady();
  checkCollisions();

  // conveyor belt - push mario
  if (mario_->on_ground)
  {
    for (const auto& p : level_->platforms)
    {
      if (p.conveyor_speed == 0.0f)
        continue;
      SDL_FRect pr = p.rect();
      SDL_FRect mr = mario_->rect();
      if (mr.x + mr.w > pr.x && mr.x < pr.x + pr.w && mario_->y + mario_->height >= p.y &&
          mario_->y + mario_->height <= p.y + p.height + 2)
      {
        mario_->x += p.conveyor_speed * Constants::FIXED_DT;
      }
    }
  }

  // Rivet Collection (level 100m)
  for (auto& r : level_->rivets)
  {
    if (!r.collected && mario_->x + mario_->width > r.x && mario_->x < r.x + r.width &&
        mario_->y + mario_->height > r.y && mario_->y < r.y + r.height)
    {
      r.collected = true;
      score_.addPoints(Constants::POINTS_RIVET);
    }
  }

  // Win cond
  if (level_->usesRivets())
  {
    if (level_->allRivetsGone())
    {
      score_.addPoints(Constants::POINTS_LEVEL_CLEAR);
      state_timer_ = Constants::LEVEL_COMPLETE_DELAY;
      state_ = GameState::LEVEL_COMPLETE;
    }
  }
  else
  {
    if (mario_->overlaps(*pauline_))
    {
      score_.addPoints(Constants::POINTS_LEVEL_CLEAR);
      escape_timer_ = 0.0f;
      dk_->startEscape();
      state_ = GameState::DK_ESCAPING;
    }
  }

  // Death handle
  if (mario_->deathDone())
  {
    if (score_.isGameOver())
    {
      state_ = GameState::GAME_OVER;
    }
    else
    {
      resetEntities();
    }
  }
}

// State: DK_ESCAPING
void Game::updateEscaping(float dt)
{
  constexpr float ESCAPE_SPEED = 80.0f;

  dk_->y -= ESCAPE_SPEED * dt;
  pauline_->x = dk_->x + dk_->width * 0.5f - pauline_->width * 0.5f;
  pauline_->y = dk_->y - pauline_->height;

  escape_timer_ += dt;
  if (escape_timer_ >= 2.0f)
  {
    state_timer_ = Constants::LEVEL_COMPLETE_DELAY;
    state_ = GameState::LEVEL_COMPLETE;
  }
}

// State: LEVEL_COMPLETE
void Game::updateLevelComplete(float dt)
{
  state_timer_ -= dt;
  if (state_timer_ <= 0.0f)
  {
    ++current_level_;
    if (current_level_ >= 3)
    {
      state_ = GameState::WIN;
    }
    else
    {
      loadLevel(current_level_);
      state_ = GameState::PLAYING;
    }
  }
}

// ── State: GAME_OVER
void Game::updateGameOver()
{
  if (input_.justPressed(SDL_SCANCODE_RETURN))
  {
    current_level_ = 0;
    score_.reset();
    loadLevel(current_level_);
    state_ = GameState::PLAYING;
  }
}

// ── State: WIN
void Game::updateWin()
{
  if (input_.justPressed(SDL_SCANCODE_RETURN))
    state_ = GameState::MENU;
}

// Render

void Game::render()
{
  if (state_ == GameState::MENU)
  {
    ui_->renderMenu(renderer_);
    SDL_RenderPresent(renderer_);
    return;
  }

  // Clear with level background
  if (level_)
    level_->renderBackground(renderer_);

  if (state_ == GameState::PLAYING || state_ == GameState::DK_ESCAPING)
  {
    // Static geometry
    for (const auto& p : level_->platforms)
      p.render(renderer_);
    for (const auto& l : level_->ladders)
      l.render(renderer_);
    for (const auto& r : level_->rivets)
      r.render(renderer_);

    // Entities
    dk_->render(renderer_);
    pauline_->render(renderer_);
    for (auto& b : barrels_)
      b.render(renderer_);
    for (auto& f : level_->fire_enemies)
      f.render(renderer_);
    mario_->render(renderer_);

    ui_->renderHUD(score_, LEVEL_NUMS[current_level_]);

    if (state_ == GameState::LEVEL_COMPLETE)
    {
      int next = (current_level_ + 1 < 3) ? LEVEL_NUMS[current_level_ + 1] : 0;
      ui_->renderLevelComplete(renderer_, next);
    }
  }
  else if (state_ == GameState::GAME_OVER)
  {
    ui_->renderGameOver(renderer_);
  }
  else if (state_ == GameState::WIN)
  {
    ui_->renderWin(renderer_, score_.score);
  }

  SDL_RenderPresent(renderer_);
}

// Helpers
void Game::loadLevel(int index)
{
  barrels_.clear();

  switch (index)
  {
    case 0:
      level_ = std::make_unique<Level25m>();
      break;
    case 1:
      level_ = std::make_unique<Level50m>();
      break;
    case 2:
      level_ = std::make_unique<Level100m>();
      break;
    default:
      return;
  }

  resetEntities();
}

void Game::resetEntities()
{
  barrels_.clear();
  mario_ = std::make_unique<Mario>(level_->mario_start_x, level_->mario_start_y);
  dk_ = std::make_unique<DonkeyKong>(level_->dk_x, level_->dk_y);
  pauline_ = std::make_unique<Pauline>(level_->pauline_x, level_->pauline_y);
}

void Game::spawnBarrelIfReady()
{
  if (!dk_ || level_->usesRivets())
    return;
  if (dk_->readyToSpawn())
  {
    float  spawn_y = level_->platforms.back().y - Constants::BARREL_H;
    Barrel b(dk_->spawnX(), spawn_y, 1.0f);
    b.on_ground = true;
    b.prev_on_ground = true;
    barrels_.push_back(std::move(b));
  }
}

void Game::checkCollisions()
{
  if (!mario_ || mario_->isDead())
    return;

  for (const auto& b : barrels_)
  {
    if (b.active && mario_->overlaps(b))
    {
      score_.loseLife();
      mario_->kill();
      return;
    }
  }

  for (const auto& f : level_->fire_enemies)
  {
    if (f.active && mario_->overlaps(f))
    {
      score_.loseLife();
      mario_->kill();
      return;
    }
  }
}
