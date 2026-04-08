#include "entities/DonkeyKong.hpp"

#include "SDL2/SDL_rect.h"
#include "SDL2/SDL_render.h"
#include "core/Constants.hpp"
#include "entities/Entity.hpp"

DonkeyKong::DonkeyKong(float x, float y) : Entity(x, y, Constants::DK_W, Constants::DK_H) {}

void DonkeyKong::update(float dt)
{
  if (escape_mode)
    return;

  barrel_timer += dt;
  // chest beat b4 a barrel spawns
  anim_timer += dt;
  if (anim_timer >= 0.15f)
  {
    anim_timer = 0.0f;
    beating_chest = !beating_chest;
  }
}

bool DonkeyKong::readyToSpawn()
{
  if (barrel_timer >= Constants::DK_BARREL_INTERVAL)
  {
    barrel_timer = 0.0f;
    return true;
  }
  return false;
}

void DonkeyKong::render(SDL_Renderer* renderer)
{
  if (!active)
    return;

  // Cosmetic barrel stack — 2x2 grid against left wall
  float bx = x - 56.0f;
  float by = y + height - 16.0f;  // bottom row sits on platform surface
  SDL_SetRenderDrawColor(renderer, 200, 150, 50, 255);
  for (int row = 0; row < 2; ++row)
    for (int col = 0; col < 2; ++col)
    {
      SDL_FRect b = {bx + col * 22.0f, by - row * 18.0f, 18.0f, 16.0f};
      SDL_RenderFillRectF(renderer, &b);
    }
  SDL_SetRenderDrawColor(renderer, 140, 90, 20, 255);
  for (int row = 0; row < 2; ++row)
    for (int col = 0; col < 2; ++col)
    {
      SDL_FRect band = {bx + col * 22.0f, by - row * 18.0f + 6.0f, 18.0f, 3.0f};
      SDL_RenderFillRectF(renderer, &band);
    }

  // body
  SDL_SetRenderDrawColor(renderer, 101, 67, 33, 255);
  SDL_FRect body = rect();
  SDL_RenderFillRectF(renderer, &body);

  // Chest highlight when beating
  if (beating_chest)
  {
    SDL_SetRenderDrawColor(renderer, 140, 90, 50, 255);
    SDL_FRect chest = {x + 16, y + 20, 32, 24};
    SDL_RenderFillRectF(renderer, &chest);
  }

  // Face
  SDL_SetRenderDrawColor(renderer, 210, 160, 100, 255);
  SDL_FRect face = {x + 12, y + 4, 40, 28};
  SDL_RenderFillRectF(renderer, &face);

  // Eyes — white with black pupils
  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
  SDL_FRect eye_l = {x + 16, y + 10, 8, 8};
  SDL_FRect eye_r = {x + 40, y + 10, 8, 8};
  SDL_RenderFillRectF(renderer, &eye_l);
  SDL_RenderFillRectF(renderer, &eye_r);
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_FRect pupil_l = {x + 18, y + 12, 4, 4};
  SDL_FRect pupil_r = {x + 42, y + 12, 4, 4};
  SDL_RenderFillRectF(renderer, &pupil_l);
  SDL_RenderFillRectF(renderer, &pupil_r);
}