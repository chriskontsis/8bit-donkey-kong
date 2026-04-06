#pragma once

#include <string>
#include <vector>

#include "SDL2/SDL_render.h"
#include "entities/FireEnemy.hpp"
#include "geometry/Ladder.hpp"
#include "geometry/Platform.hpp"
#include "geometry/Rivet.hpp"

// abstract base - each level owns is geom and start pos
// game holds up<Level> call renderBackground() each frame

class Level
{
 public:
  std::vector<Platform>  platforms;
  std::vector<Ladder>    ladders;
  std::vector<FireEnemy> fire_enemies;
  std::vector<Rivet>     rivets;

  float mario_start_x = 0, mario_start_y = 0;
  float dk_x = 0, dk_y = 0;
  float pauline_x = 0, pauline_y = 0;

  // level 100m win cond - all riv collected
  bool usesRivets() const { return !rivets.empty(); }
  bool allRivetsGone() const
  {
    for (const auto& r : rivets)
    {
      if (!r.collected)
        return false;
    }
    return true;
  }

  virtual ~Level() = default;
  virtual void renderBackground(SDL_Renderer* renderer) = 0;
  virtual std::string name() const = 0;
};