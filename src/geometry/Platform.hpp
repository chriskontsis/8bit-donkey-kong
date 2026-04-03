#pragma once

#include <SDL2/SDL.h>

#include "SDL2/SDL_rect.h"
#include "SDL2/SDL_render.h"

// static geometry, no velocity, no v dispatch
struct Platform
{
  float     x, y, width, height;
  float     conveyor_speed = 0.0f;  // px/s + pushes right, - pushes left
  SDL_Color color{180, 120, 60, 255};

  SDL_FRect rect() const { return {x, y, width, height}; }
  void render(SDL_Renderer* r) const
  {
    SDL_SetRenderDrawColor(r, color.r, color.g, color.b, color.a);
    SDL_FRect rc = rect();
    SDL_RenderFillRectF(r, &rc);

    // conveyor stripe - visual indicator when belt is active
    if (conveyor_speed != 0.0f)
    {
      SDL_SetRenderDrawColor(r, 255, 220, 50, 180);
      for (float sx = x; sx < x + width; sx += 24.0f)
      {
        SDL_FRect stripe = {sx, y + 4, 12, height - 8};
        SDL_RenderFillRectF(r, &stripe);
      }
    }
  }
};