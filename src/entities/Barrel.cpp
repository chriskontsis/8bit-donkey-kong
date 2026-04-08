#include "entities/Barrel.hpp"

#include <cstdlib>
#include <vector>

#include "core/Constants.hpp"

Barrel::Barrel(float x, float y, float dir) : Entity(x, y, Constants::BARREL_W, Constants::BARREL_H)
{
  vx = dir * Constants::BARREL_ROLL_SPEED;
  prev_y = y;
}

void Barrel::update(float dt, const std::vector<Platform>& platforms,
                    const std::vector<Ladder>& ladders)
{
  // descending on ladder
  if (on_ladder)
  {
    y += Constants::BARREL_DESCEND_SPD * dt;
    if (y + height >= ladder_exit_y)
    {
      y = ladder_exit_y - height;
      on_ladder = false;
      on_ground = true;
      prev_on_ground = true;
      vx = saved_vx;
      vy = 0.0f;
      ladder_cooldown = 0.5f;
    }
    if (x + width < 0 || x > Constants::WINDOW_WIDTH || y > Constants::WINDOW_HEIGHT + 100)
      active = false;
    return;
  }

  if (ladder_cooldown > 0.0f)
    ladder_cooldown -= dt;

  prev_y = y;
  prev_on_ground = on_ground;
  on_ground = false;

  vy += Constants::GRAVITY * dt;
  y += vy * dt;
  resolveY(platforms);
  x += vx * dt;

  // ── Check for ladder descent when rolling on a platform
  if (on_ground && ladder_cooldown <= 0.0f)
  {
    float cx = x + width * 0.5f;
    for (const auto& l : ladders)
    {
      float barrel_bottom = y + height;
      if (cx >= l.x && cx <= l.x + l.width && barrel_bottom > l.y - 4.0f &&
          barrel_bottom < l.y + 4.0f)
      {
        if (std::rand() % 3 == 0)
        {
          on_ladder = true;
          saved_vx = vx;
          vx = 0.0f;
          vy = 0.0f;
          ladder_exit_y = l.y + l.height;
        }
        else
        {
          ladder_cooldown = 0.8f;  // skip this ladder, check again later
        }
        break;
      }
    }
  }

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
    if (prev_bottom <= p.y && cur_bottom >= p.y && !p.hasHoleAt(x, width))
    {
      y = p.y - height;
      vy = 0.0f;
      on_ground = true;

      // rev horiz dir on landing
      if (!prev_on_ground)
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
