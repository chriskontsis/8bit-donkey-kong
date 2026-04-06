#include "entities/Barrel.hpp"

#include <vector>

#include "core/Constants.hpp"
#include "entities/Entity.hpp"
#include "geometry/Platform.hpp"

Barrel::Barrel(float x, float y, float dir) : Entity(x, y, Constants::BARREL_W, Constants::BARREL_H)
{
  vx = dir * Constants::BARREL_ROLL_SPEED;
  prev_y = y;
}

void Barrel::update(float dt, const std::vector<Platform>& platforms)
{
  prev_y = y;
  was_in_air = !on_ground;
  on_ground = false;

  vy += Constants::GRAVITY * dt;

  y += vy * dt;
  resolveY(platforms);

  x += vx * dt;

  // remove when off screen
  if (x + width < 0 || x > Constants::WINDOW_WIDTH || y > Constants::WINDOW_HEIGHT + 100)
  {
    active = false;
    return;
  }

  roll_angle += vx * dt * 3.0f;
}

void Barrel::resolveY(const std::vector<Platform>& platforms)
{
  float prev_bottom = prev_y + height;
  float cur_bottom = y + height;

  for (const auto& p : platforms)
  {
    if (x + width <= p.x || x >= p.x + p.width)
      continue;

    // landing on top of platform
    if (prev_bottom <= p.y && cur_bottom >= p.y)
    {
      y = p.y - height;
      vy = 0.0f;
      on_ground = true;

      // rev horiz dir on landing
      if (was_in_air)
        vx = -vx;
    }
  }
}

void Barrel::render(SDL_Renderer* renderer)
{
  if (!active)
    return;

  // Outer barrel — yellow/brown
  SDL_SetRenderDrawColor(renderer, 200, 150, 50, 255);
  SDL_FRect body = rect();
  SDL_RenderFillRectF(renderer, &body);

  // Rolling band — alternates position based on roll_angle
  SDL_SetRenderDrawColor(renderer, 140, 90, 20, 255);
  float     band_offset = (static_cast<int>(roll_angle) % 20) / 20.0f * height;
  SDL_FRect band = {x, y + band_offset, width, 4};
  SDL_RenderFillRectF(renderer, &band);
}
