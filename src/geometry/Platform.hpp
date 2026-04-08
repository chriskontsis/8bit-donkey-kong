#pragma once

#include <SDL2/SDL.h>

#include <optional>

#include "SDL2/SDL_rect.h"
#include "SDL2/SDL_render.h"

struct Platform
{
  struct Hole
  {
    float x;
    float width;
  };

  float     x, y, width, height;
  float     conveyor_speed = 0.0f;  // px/s + pushes right, - pushes left
  SDL_Color color{180, 120, 60, 255};
  float     slope = 0.0f;  // visual tilt

  // holes left by collected rivets in 100m
  std::optional<Hole> hole1;
  std::optional<Hole> hole2;
  float hole_open_delay = 0.0f;  // allow mario to collect and not drop through in same frame

  SDL_FRect rect() const { return {x, y, width, height}; }

  bool hasHoleAt(float ex, float ew) const
  {
    if (hole_open_delay > 0.0f)
      return false;
    auto overlaps = [&](const std::optional<Hole>& h)
    { return h && ex + ew > h->x && ex < h->x + h->width; };

    return overlaps(hole1) || overlaps(hole2);
  }

  void render(SDL_Renderer* r) const
  {
    SDL_SetRenderDrawColor(r, color.r, color.g, color.b, color.a);

    if (slope == 0.0f)
    {
      SDL_FRect rc = rect();
      SDL_RenderFillRectF(r, &rc);
    }
    else
    {
      // Draw as slanted parallelogram. new strip each time y - offset steps by 1px
      int   prev_off = 0;
      float strip_start = 0.0f;
      for (float px = 1.0f; px <= width + 1.0f; ++px)
      {
        int cur_off = static_cast<int>(slope * px);
        if (cur_off != prev_off || px > width)
        {
          SDL_FRect strip = {x + strip_start, y + prev_off, px - strip_start, height};
          SDL_RenderFillRectF(r, &strip);
          strip_start = px;
          prev_off = cur_off;
        }
      }
    }

    // draw holes as black gaps
    SDL_SetRenderDrawColor(r, 0, 0, 0, 255);
    if (hole1)
    {
      SDL_FRect h = {hole1->x, y, hole1->width, height};
      SDL_RenderFillRectF(r, &h);
    }
    if (hole2)
    {
      SDL_FRect h = {hole2->x, y, hole2->width, height};
      SDL_RenderFillRectF(r, &h);
    }

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