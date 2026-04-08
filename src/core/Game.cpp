#include "core/Game.hpp"

#include <SDL2/SDL_video.h>

#include <algorithm>
#include <memory>
#include <ranges>

#include "SDL2/SDL.h"
#include "SDL2/SDL_render.h"
#include "SDL2/SDL_scancode.h"
#include "SDL2/SDL_timer.h"
#include "core/Constants.hpp"
#include "core/GameState.hpp"
#include "geometry/Platform.hpp"
#include "levels/Level100m.hpp"
#include "levels/Level25m.hpp"
#include "levels/Level50m.hpp"

// Init
std::expected<void, std::string> Game::init()
{
  if (SDL_Init(SDL_INIT_VIDEO) != 0)
    return std::unexpected{"SDL_Init failed: " + std::string{SDL_GetError()}};

  window_.reset(SDL_CreateWindow("Donkey Kong", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                 Constants::WINDOW_WIDTH, Constants::WINDOW_HEIGHT,
                                 SDL_WINDOW_SHOWN));
  if (!window_)
    return std::unexpected{"SDL_CreateWindow failed: " + std::string{SDL_GetError()}};

  renderer_.reset(
      SDL_CreateRenderer(window_.get(), -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC));
  if (!renderer_)
    return std::unexpected{"SDL_CreateRenderer failed: " + std::string{SDL_GetError()}};

  ui_ = std::make_unique<UI>(renderer_.get());
  return {};
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
    current_level_ = 2;
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
  std::erase_if(barrels_, [](const Barrel& b) { return !b.active; });

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

  // Tick hole open delays
  for (auto& p : level_->platforms)
    if (p.hole_open_delay > 0.0f)
      p.hole_open_delay -= dt;

  // Rivet Collection (level 100m)
  for (auto& r : level_->rivets)
  {
    if (!r.collected && mario_->x + mario_->width > r.x && mario_->x < r.x + r.width &&
        mario_->y + mario_->height >= r.y && mario_->y < r.y + r.height)
    {
      r.collected = true;
      score_.addPoints(Constants::POINTS_RIVET);
      if (r.platform_idx >= 0 && r.platform_idx < static_cast<int>(level_->platforms.size()))
      {
        auto& p = level_->platforms[r.platform_idx];
        if (!p.hole1)
          p.hole1 = Platform::Hole(r.x, r.width);
        else
          p.hole2 = Platform::Hole(r.x, r.width);
        p.hole_open_delay = 1.2f;  // grace period before hole is passable
      }
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
    ui_->renderMenu(renderer_.get());
    SDL_RenderPresent(renderer_.get());
    return;
  }

  // Clear with level background
  if (level_)
    level_->renderBackground(renderer_.get());

  if (state_ == GameState::PLAYING || state_ == GameState::DK_ESCAPING)
  {
    // Static geometry
    for (const auto& p : level_->platforms)
      p.render(renderer_.get());
    for (const auto& l : level_->ladders)
      l.render(renderer_.get());
    for (const auto& r : level_->rivets)
      r.render(renderer_.get());

    // Entities
    dk_->render(renderer_.get());
    pauline_->render(renderer_.get());
    for (auto& b : barrels_)
      b.render(renderer_.get());
    for (auto& f : level_->fire_enemies)
      f.render(renderer_.get());
    mario_->render(renderer_.get());

    ui_->renderHUD(score_, LEVEL_NUMS[current_level_]);

    if (state_ == GameState::LEVEL_COMPLETE)
    {
      int next = (current_level_ + 1 < 3) ? LEVEL_NUMS[current_level_ + 1] : 0;
      ui_->renderLevelComplete(renderer_.get(), next);
    }
  }
  else if (state_ == GameState::GAME_OVER)
  {
    ui_->renderGameOver(renderer_.get());
  }
  else if (state_ == GameState::WIN)
  {
    ui_->renderWin(renderer_.get(), score_.score);
  }

  SDL_RenderPresent(renderer_.get());
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

  auto hitsMario = [&](const auto& e) { return e.active && mario_->overlaps(e); };

  if (std::ranges::any_of(barrels_, hitsMario) ||
      std::ranges::any_of(level_->fire_enemies, hitsMario))
  {
    score_.loseLife();
    mario_->kill();
  }
}
