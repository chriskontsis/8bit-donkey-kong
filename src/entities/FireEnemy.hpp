#pragma once
#include <vector>

#include "entities/Entity.hpp"
#include "geometry/Platform.hpp"

class FireEnemy : public Entity
{
 public:
  float anim_timer = 0.0f;
  bool  flicker = false;
  float patrol_left = 0.0f;
  float patrol_right = 0.0f;

  FireEnemy(float x, float y, float patrol_left, float patrol_right);
  void update(float dt);
  void render(SDL_Renderer* renderer) override;
};