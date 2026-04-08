#include "levels/Level25m.hpp"

#include "core/Constants.hpp"

Level25m::Level25m()
{
  const float W = Constants::WINDOW_WIDTH;   // 768
  const float H = Constants::WINDOW_HEIGHT;  // 672

  SDL_Color brown{180, 120, 60, 255};

  // ── Platforms — ground first, DK platform last (platforms.back() used for barrel spawn)
  platforms.push_back({0, 640, W, 32, 0.0f, brown, 0.0f});       // ground
  platforms.push_back({0, 560, 580, 16, 0.0f, brown, 0.01f});    // row 1 —
  platforms.push_back({188, 480, 580, 16, 0.0f, brown, 0.01f});  // row 2 —
  platforms.push_back({0, 400, 580, 16, 0.0f, brown, 0.01f});    // row 3 —
  platforms.push_back({188, 320, 580, 16, 0.0f, brown, 0.01f});  // row 4 —
  platforms.push_back({0, 240, 580, 16, 0.0f, brown, 0.01f});    // row 5 —
  platforms.push_back({188, 160, 580, 16, 0.0f, brown, 0.01f});  // row 6 —
  platforms.push_back({0, 80, 460, 16, 0.0f, brown, 0.01f});     // DK

  ladders.push_back({20, 0, 20, 80});

  ladders.push_back({200, 80, 20, 80});
  ladders.push_back({400, 80, 20, 80});

  ladders.push_back({250, 160, 20, 80});
  ladders.push_back({540, 160, 20, 80});

  ladders.push_back({200, 240, 20, 80});
  ladders.push_back({500, 240, 20, 80});

  ladders.push_back({320, 320, 20, 80});
  ladders.push_back({540, 320, 20, 80});

  ladders.push_back({200, 400, 20, 80});
  ladders.push_back({480, 400, 20, 80});

  ladders.push_back({260, 480, 20, 80});
  ladders.push_back({520, 480, 20, 80});

  ladders.push_back({80, 560, 20, 80});
  ladders.push_back({460, 560, 20, 80});

  // ── Entity starting positions
  dk_x = 60.0f;
  dk_y = 80.0f - Constants::DK_H;
  pauline_x = 300.0f;
  pauline_y = 80.0f - Constants::PAULINE_H;
  mario_start_x = W - 80.0f;
  mario_start_y = 640.0f - Constants::MARIO_H;
}

void Level25m::renderBackground(SDL_Renderer* renderer)
{
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderClear(renderer);
}