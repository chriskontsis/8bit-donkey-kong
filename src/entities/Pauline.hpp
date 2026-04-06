#pragma once

#include "entities/Entity.hpp"

class Pauline : public Entity
{
 public:
  float anim_timer = 0.0f;
  bool  waving = false;

  Pauline(float x, float y);

  void update(float dt);
  void render(SDL_Renderer* renderer) override;
};