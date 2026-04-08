#pragma once

#include <SDL2/SDL.h>

#include "SDL2/SDL_rect.h"
#include "SDL2/SDL_render.h"

struct Rivet
{
  float x, y, width, height;
  bool  collected = false;
  int   platform_idx = -1;  // index into level_->platforms; - 1 = unlinked

  SDL_FRect rect() const { return {x, y, width, height}; }

  void render(SDL_Renderer* r) const
  {
    if (collected)
      return;

    // Golden bolt embedded in platform
    SDL_SetRenderDrawColor(r, 200, 130, 0, 255);
    SDL_FRect rc = rect();
    SDL_RenderFillRectF(r, &rc);

    SDL_SetRenderDrawColor(r, 255, 200, 50, 255);
    SDL_FRect highlight = {x + 4, y + 4, width - 8, height - 8};
    SDL_RenderFillRectF(r, &highlight);
  }
};