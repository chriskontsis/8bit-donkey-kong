#include "entities/DonkeyKong.hpp"

#include "SDL2/SDL_rect.h"
#include "SDL2/SDL_render.h"
#include "core/Constants.hpp"
#include "entities/Entity.hpp"

DonkeyKong::DonkeyKong(float x, float y) : Entity(x, y, Constants::DK_W, Constants::DK_H) {}

void DonkeyKong::update(float dt)
{
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

  // Body
  SDL_SetRenderDrawColor(renderer, 101, 67, 33, 255);
  SDL_FRect body = {x, y + height * 0.3f, width, height * 0.7f};
  SDL_RenderFillRectF(renderer, &body);

  // Head
  SDL_SetRenderDrawColor(renderer, 120, 80, 40, 255);
  SDL_FRect head = {x + 8, y, width - 16, height * 0.38f};
  SDL_RenderFillRectF(renderer, &head);

  // Eyes
  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
  SDL_FRect eye_l = {x + 12, y + 8, 8, 8};
  SDL_FRect eye_r = {x + width - 20, y + 8, 8, 8};
  SDL_RenderFillRectF(renderer, &eye_l);
  SDL_RenderFillRectF(renderer, &eye_r);

  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_FRect pupil_l = {x + 14, y + 10, 4, 4};
  SDL_FRect pupil_r = {x + width - 18, y + 10, 4, 4};
  SDL_RenderFillRectF(renderer, &pupil_l);
  SDL_RenderFillRectF(renderer, &pupil_r);

  // Arms — raised higher when beating chest
  float arm_y = beating_chest ? y + height * 0.25f : y + height * 0.35f;
  SDL_SetRenderDrawColor(renderer, 101, 67, 33, 255);
  SDL_FRect arm_l = {x - 12, arm_y, 14, 18};
  SDL_FRect arm_r = {x + width - 2, arm_y, 14, 18};
  SDL_RenderFillRectF(renderer, &arm_l);
  SDL_RenderFillRectF(renderer, &arm_r);
}