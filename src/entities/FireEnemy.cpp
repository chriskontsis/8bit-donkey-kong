#include "entities/FireEnemy.hpp"

#include <cmath>

#include "core/Constants.hpp"
#include "entities/Entity.hpp"

FireEnemy::FireEnemy(float x, float y, float p_left, float p_right)
    : Entity(x, y, Constants::FIRE_W, Constants::FIRE_H), patrol_left(p_left), patrol_right(p_right)
{
  vx = Constants::BARREL_ROLL_SPEED * 0.6f;  // slower than barrels
}

void FireEnemy::update(float dt)
{
  anim_timer += dt;

  // flicker anim
  if (anim_timer >= 0.12f)
  {
    anim_timer = 0.0f;
    flicker = !flicker;
  }

  // patrol back and forth between bounds
  x += vx * dt;
  if (x <= patrol_left)
  {
    x = patrol_left;
    vx = std::abs(vx);
  }
  if (x >= patrol_right)
  {
    x = patrol_right;
    vx = -std::abs(vx);
  }
}

void FireEnemy::render(SDL_Renderer* renderer)
{
  if (!active)
    return;

  // Alternates between orange and red for flame flicker
  if (flicker)
    SDL_SetRenderDrawColor(renderer, 255, 120, 0, 255);  // orange
  else
    SDL_SetRenderDrawColor(renderer, 220, 40, 0, 255);  // red

  SDL_FRect body = rect();
  SDL_RenderFillRectF(renderer, &body);

  // Bright yellow core
  SDL_SetRenderDrawColor(renderer, 255, 240, 50, 255);
  SDL_FRect core = {x + 5, y + 5, width - 10, height - 10};
  SDL_RenderFillRectF(renderer, &core);
}
