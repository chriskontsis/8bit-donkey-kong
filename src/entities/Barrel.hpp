#pragma once
#include <vector>

#include "entities/Entity.hpp"
#include "geometry/Platform.hpp"

class Barrel : public Entity
{
 public:
  bool  on_ground = false;
  bool  was_in_air = true;  // used 4 landing and rev dir
  float roll_angle = 0.0f;  // visual rotation
  float prev_y = 0.0f;

  Barrel(float x, float y, float dir);  // dir: +1 = R, -1 = L
  void update(float dt, const std::vector<Platform>& platforms);
  void render(SDL_Renderer* renderer) override;

 private:
  void resolveY(const std::vector<Platform>& platforms);
};