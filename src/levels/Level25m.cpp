#include "levels/Level25m.hpp"

#include "core/Constants.hpp"

Level25m::Level25m()
{
  const float W = Constants::WINDOW_WIDTH;
  const float H = Constants::WINDOW_HEIGHT;

  // ── Platforms ───────────────────────────────
  SDL_Color brown{180, 120, 60, 255};

  platforms.push_back({0, H - 32, W, 32, 0.0f, brown});          // ground
  platforms.push_back({60, H - 128, W - 110, 16, 0.0f, brown});  // row 1
  platforms.push_back({0, H - 224, W - 110, 16, 0.0f, brown});   // row 2
  platforms.push_back({60, H - 320, W - 110, 16, 0.0f, brown});  // row 3
  platforms.push_back({0, H - 416, W - 110, 16, 0.0f, brown});   // row 4
  platforms.push_back({0, H - 512, 440, 16, 0.0f, brown});       // top

  // ── Ladders ─────────────────────────────────
  ladders.push_back({640, H - 128, 20, 96});
  // ground → row1  (right)
  ladders.push_back({100, H - 224, 20, 96});
  // row1   → row2  (left)
  ladders.push_back({570, H - 320, 20, 96});
  // row2   → row3  (right)
  ladders.push_back({100, H - 416, 20, 96});
  // row3   → row4  (left)
  ladders.push_back({380, H - 512, 20, 96});
  // row4   → top   (mid)

  // ── Starting positions
  dk_x = 20;
  dk_y = H - 512 - Constants::DK_H;
  pauline_x = 340;
  pauline_y = H - 512 - Constants::PAULINE_H;
  mario_start_x = W - 80 - Constants::MARIO_W;
  mario_start_y = H - 32 - Constants::MARIO_H;
}

void Level25m::renderBackground(SDL_Renderer* renderer)
{
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderClear(renderer);
}