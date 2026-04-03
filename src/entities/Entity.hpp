#pragma once
#include <SDL2/SDL.h>

#include "SDL2/SDL_rect.h"
#include "SDL2/SDL_render.h"

// abstact base for all moveable objects

class Entity
{
 public:
  float x, y;
  float vx = 0.0f, vy = 0.0f;
  float width, height;
  bool  active = true;

  Entity(float x, float y, float w, float h) : x(x), y(y), width(w), height(h) {}
  virtual ~Entity() = default;

  virtual void render(SDL_Renderer* renderer) = 0;

  // AABB overlap to detect collision
  bool overlaps(const Entity& other) const
  {
    return x < other.x + other.width && x + width > other.x && y < other.y + other.height &&
           y + height > other.y;
  }

  SDL_FRect rect() const { return {x, y, width, height}; }
  float centerX() const { return x + width * 0.5f; }
  float centerY() const { return y + height * 0.5f; }
};