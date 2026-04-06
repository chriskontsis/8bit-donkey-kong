#pragma once

#include "levels/Level.hpp"

class Level25m : public Level
{
 public:
  Level25m();
  void renderBackground(SDL_Renderer* renderer) override;
  std::string name() const override { return "25m"; }
};