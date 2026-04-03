#pragma once

#include <SDL2/SDL.h>

#include "SDL2/SDL_rect.h"
#include "SDL2/SDL_render.h"

struct Ladder
{
  float x, y, width, height;
  SDL_FRect rect() const { return {x, y, width, height}; }

  void render(SDL_Renderer* r) const
  {
    // two vertical rails
    SDL_SetRenderDrawColor(r, 200, 200, 50, 255);
    SDL_FRect rail1 = {x,             y, 4, height};
    SDL_FRect rail2 = {x + width - 4, y, 4, height};
    SDL_RenderFillRectF(r, &rail1);
    SDL_RenderFillRectF(r, &rail2);

    // horizontal rungs every 16 px
    for (float ry = y; ry < y + height; ry += 16.0f)
    {
      SDL_FRect rung = {x, ry, width, 3};
      SDL_RenderFillRectF(r, &rung);
    }
  }
};