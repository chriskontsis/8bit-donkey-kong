#include "entities/Mario.hpp"

#include "SDL2/SDL_scancode.h"
#include "core/Constants.hpp"
#include "entities/Entity.hpp"
#include "geometry/Ladder.hpp"
#include "geometry/Platform.hpp"
#include "input/InputHandler.hpp"

Mario::Mario(float x, float y) : Entity(x, y, Constants::MARIO_W, Constants::MARIO_H)
{
  prev_y = y;
}

void Mario::update(float dt, const InputHandler& input, const std::vector<Platform>& platforms,
                   const std::vector<Ladder>& ladders)
{
  if (state == MarioState::DEAD)
  {
    death_timer -= dt;
    return;
  }

  prev_y = y;
  was_on_ground = on_ground;
  on_ground = false;
  was_on_ladder = on_ladder;
  on_ladder = isOverLadder(ladders);

  handleInput(input, ladders);

  if (!on_ladder)
    applyGravity(dt);

  y += vy * dt;
  resolveY(platforms);

  x += vx * dt;
  clampToScreen();

  // update state
  if (on_ladder && vy != 0.0f)
    state = MarioState::CLIMBING;
  else if (!on_ground && !on_ladder)
    state = MarioState::JUMPING;
  else if (vx != 0.0f)
    state = MarioState::RUNNING;
  else
    state = MarioState::IDLE;
}

void Mario::applyGravity(float dt)
{
  vy += Constants::GRAVITY * dt;
}

void Mario::handleInput(const InputHandler& input, const std::vector<Ladder>& ladders)
{
  vx = 0.0f;

  if (input.justPressed(SDL_SCANCODE_SPACE) && was_on_ground)
  {
    vy = Constants::MARIO_JUMP_VY;
    on_ground = false;
    return;
  }

  if (on_ladder)
  {
    if (input.isHeld(SDL_SCANCODE_UP))
      vy = -Constants::MARIO_CLIMB_SPD;
    else if (input.isHeld(SDL_SCANCODE_DOWN))
      vy = Constants::MARIO_CLIMB_SPD;
    else
      vy = 0.0f;

    if (input.isHeld(SDL_SCANCODE_LEFT))
    {
      vx = -Constants::MARIO_RUN_SPEED;
      facing_dir = -1;
    }
    if (input.isHeld(SDL_SCANCODE_RIGHT))
    {
      vx = Constants::MARIO_RUN_SPEED;
      facing_dir = 1;
    }
  }
  else
  {
    if (input.isHeld(SDL_SCANCODE_LEFT))
    {
      vx = -Constants::MARIO_RUN_SPEED;
      facing_dir = -1;
    }
    if (input.isHeld(SDL_SCANCODE_RIGHT))
    {
      vx = Constants::MARIO_RUN_SPEED;
      facing_dir = 1;
    }
    if (input.justPressed(SDL_SCANCODE_SPACE) && was_on_ground)
    {
      vy = Constants::MARIO_JUMP_VY;
      on_ground = false;
    }
  }
}

void Mario::resolveY(const std::vector<Platform>& platforms)
{
  float prev_bottom = prev_y + height;
  float cur_bottom = y + height;

  for (const auto& p : platforms)
  {
    // must overlap x axis
    if (x + width <= p.x || x >= p.x + p.width)
      continue;

    // landing on top
    if (prev_bottom <= p.y && cur_bottom >= p.y)
    {
      y = p.y - height;
      vy = 0.0f;
      on_ground = true;
    }

    // hitting underside
    else if (!on_ladder && prev_y >= p.y + p.height && y <= p.y + p.height)
    {
      y = p.y + p.height;
      vy = 0.0f;
    }

    // Climbing up onto platform via ladder
    else if (was_on_ladder && vy < 0 && prev_y + height > p.y && y + height <= p.y)
    {
      y = p.y - height;
      vy = 0.0f;
      on_ground = true;
    }
  }
}

void Mario::clampToScreen()
{
  if (x < 0)
    x = 0;
  if (x + width > Constants::WINDOW_WIDTH)
    x = Constants::WINDOW_WIDTH - width;
}

bool Mario::isOverLadder(const std::vector<Ladder>& ladders) const
{
  float cx = centerX();
  float feet = y + height;

  for (const auto& ladder : ladders)
  {
    if (cx >= ladder.x && cx <= ladder.x + ladder.width && feet >= ladder.y &&
        feet <= ladder.y + ladder.height)
      return true;
  }
  return false;
}

void Mario::kill()
{
  if (state == MarioState::DEAD)
    return;
  state = MarioState::DEAD;
  death_timer = Constants::DEATH_DELAY;
  vx = vy = 0.0f;
}

void Mario::render(SDL_Renderer* renderer)
{
  if (!active)
    return;

  bool is_dead = (state == MarioState::DEAD);

  // Body — red (darker when dead)
  SDL_SetRenderDrawColor(renderer, is_dead ? 140 : 220, 50, 50, 255);
  SDL_FRect body = {x, y + height * 0.4f, width, height * 0.6f};
  SDL_RenderFillRectF(renderer, &body);

  // Head — skin tone
  SDL_SetRenderDrawColor(renderer, 255, 200, 150, 255);
  SDL_FRect head = {x + 2, y, width - 4, height * 0.45f};
  SDL_RenderFillRectF(renderer, &head);

  // Hat — blue cap
  SDL_SetRenderDrawColor(renderer, 50, 50, 200, 255);
  SDL_FRect hat = {x, y, width, height * 0.2f};
  SDL_RenderFillRectF(renderer, &hat);

  // Eye — moves with facing direction
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  float     eye_x = (facing_dir == 1) ? x + width - 8 : x + 4;
  SDL_FRect eye = {eye_x, y + 7, 4, 4};
  SDL_RenderFillRectF(renderer, &eye);
}