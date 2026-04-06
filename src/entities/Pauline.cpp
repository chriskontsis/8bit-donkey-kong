#include "entities/Pauline.hpp"

#include "core/Constants.hpp"
#include "entities/Entity.hpp"

Pauline::Pauline(float x, float y) : Entity(x, y, Constants::PAULINE_W, Constants::PAULINE_H) {}

void Pauline::update(float dt)
{
  anim_timer += dt;
  if (anim_timer >= 0.4f)
  {
    anim_timer = 0.0f;
    waving = !waving;
  }
}

void Pauline::render(SDL_Renderer* renderer)
{
  if (!active)
    return;

  // Dress — red
  SDL_SetRenderDrawColor(renderer, 220, 50, 80, 255);
  SDL_FRect dress = {x, y + height * 0.4f, width, height * 0.6f};
  SDL_RenderFillRectF(renderer, &dress);

  // Head — skin
  SDL_SetRenderDrawColor(renderer, 255, 200, 150, 255);
  SDL_FRect head = {x + 2, y, width - 4, height * 0.38f};
  SDL_RenderFillRectF(renderer, &head);

  // Hair — dark brown
  SDL_SetRenderDrawColor(renderer, 80, 40, 10, 255);
  SDL_FRect hair = {x, y, width, height * 0.18f};
  SDL_RenderFillRectF(renderer, &hair);

  // Waving arm
  float arm_x = waving ? x + width : x + width - 4;
  float arm_y = waving ? y + height * 0.3f : y + height * 0.4f;
  SDL_SetRenderDrawColor(renderer, 255, 200, 150, 255);
  SDL_FRect arm = {arm_x, arm_y, 8, 10};
  SDL_RenderFillRectF(renderer, &arm);
}