#pragma once

#include <vector>

#include "SDL2/SDL_render.h"
#include "entities/Entity.hpp"
#include "geometry/Ladder.hpp"
#include "geometry/Platform.hpp"
#include "input/InputHandler.hpp"

enum class MarioState
{
  IDLE,
  RUNNING,
  JUMPING,
  CLIMBING,
  DEAD
};

class Mario : public Entity
{
 public:
  MarioState state = MarioState::IDLE;
  bool       on_ground = false;
  bool       on_ladder = false;
  int        facing_dir = 1;
  float      death_timer = 0.0f;
  float      prev_y = 0.0f;

  Mario(float x, float y);
  void update(float dt, const InputHandler& input, const std::vector<Platform>& platforms,
              const std::vector<Ladder>& ladders);
  void render(SDL_Renderer* renderer) override;
  void kill();
  bool isDead() const { return state == MarioState::DEAD; }
  bool deathDone() const { return state == MarioState::DEAD && death_timer <= 0.0f; }

 private:
  void applyGravity(float dt);
  void handleInput(const InputHandler& input, const std::vector<Ladder>& ladders);
  void resolveY(const std::vector<Platform>& platforms);
  void clampToScreen();
  bool isOverLadder(const std::vector<Ladder>& ladders) const;
};