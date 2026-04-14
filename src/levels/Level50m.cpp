#include "levels/Level50m.hpp"

#include "core/Constants.hpp"

Level50m::Level50m()
{
  const float W = Constants::WINDOW_WIDTH;   // 768
  SDL_Color steel{60, 90, 140, 255};

  // ── Platforms — full width, alternating conveyor direction
  platforms.push_back({0, 640, W,         32,  0.0f,   steel});  // ground
  platforms.push_back({0, 560, W,         16,  60.0f,  steel});  // row 4 — belt right
  platforms.push_back({0, 480, W,         16, -60.0f,  steel});  // row 3 — belt left
  platforms.push_back({0, 400, W,         16,  60.0f,  steel});  // row 2 — belt right
  platforms.push_back({0, 320, W,         16, -60.0f,  steel});  // row 1 — belt left
  platforms.push_back({0, 240, W * 0.6f,  16,  0.0f,   steel});  // DK platform — top left

  // ── Ladders — 2 per gap, staggered left/right so Mario has options
  // Ground → Row 4
  ladders.push_back({180, 560, 20, 80});
  ladders.push_back({560, 560, 20, 80});
  // Row 4 → Row 3
  ladders.push_back({120, 480, 20, 80});
  ladders.push_back({620, 480, 20, 80});
  // Row 3 → Row 2
  ladders.push_back({250, 400, 20, 80});
  ladders.push_back({520, 400, 20, 80});
  // Row 2 → Row 1
  ladders.push_back({150, 320, 20, 80});
  ladders.push_back({580, 320, 20, 80});
  // Row 1 → DK platform
  ladders.push_back({350, 240, 20, 80});

  // ── Fire enemies — patrol conveyor rows
  fire_enemies.emplace_back(300, 560 - Constants::FIRE_H, 60, W - 20);
  fire_enemies.emplace_back(450, 400 - Constants::FIRE_H, 60, W - 20);

  // ── Entity positions
  dk_x          = 20.0f;
  dk_y          = 240.0f - Constants::DK_H;
  pauline_x     = 120.0f;
  pauline_y     = 240.0f - Constants::PAULINE_H;
  mario_start_x = W - 80.0f;
  mario_start_y = 640.0f - Constants::MARIO_H;
}

void Level50m::renderBackground(SDL_Renderer* renderer)
{
  SDL_SetRenderDrawColor(renderer, 5, 10, 30, 255);
  SDL_RenderClear(renderer);
}
