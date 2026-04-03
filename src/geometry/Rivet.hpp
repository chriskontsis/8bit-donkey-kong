#pragma once

#include <SDL2/SDL.h>

#include "SDL2/SDL_rect.h"
#include "SDL2/SDL_render.h"

// Collectibles in 100m, collect all & DK falls
struct Rivet
{
  float x, y;
  float width    = 14.0f;
  float height   = 14.0f;
  bool  collected = false;

  SDL_FRect rect() const { return {x, y, width, height}; }
  void render(SDL_Renderer* r) const
  {
    if (collected)
      return;
    SDL_SetRenderDrawColor(r, 220, 220, 220, 255);
    SDL_FRect rc = rect();
    SDL_RenderFillRectF(r, &rc);
    SDL_SetRenderDrawColor(r, 80, 80, 80, 255);
    SDL_FRect dot = {x + 4, y + 4, 6, 6};
    SDL_RenderFillRectF(r, &dot);
  }
};