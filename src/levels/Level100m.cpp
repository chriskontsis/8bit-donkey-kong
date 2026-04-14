#include "levels/Level100m.hpp"

#include "core/Constants.hpp"

Level100m::Level100m()
{
  const float W = Constants::WINDOW_WIDTH;
  const float H = Constants::WINDOW_HEIGHT;

  SDL_Color steel{60, 100, 180, 255};

  // ── Platforms — ground first, DK platform last
  // Index: 0=ground, 1=row4, 2=row3, 3=row2, 4=row1, 5=DK
  platforms.push_back({0, 640, W, 32, 0.0f, steel});  //
  platforms.push_back({0, 528, W, 16, 0.0f, steel});  // row 4
  platforms.push_back({0, 416, W, 16, 0.0f, steel});  // row 3
  platforms.push_back({0, 304, W, 16, 0.0f, steel});  // row 2
  platforms.push_back({0, 192, W, 16, 0.0f, steel});  // row 1
  platforms.push_back({0, 80, W, 16, 0.0f, steel});   // DK platform

  // ── Ladders — 3 per gap, staggered so no column appears straight
  // DK platform → Row 1
  ladders.push_back({60,  80, 20, 112});
  ladders.push_back({330, 80, 20, 112});
  ladders.push_back({620, 80, 20, 112});

  // Row 1 → Row 2  (shift right)
  ladders.push_back({170, 192, 20, 112});
  ladders.push_back({460, 192, 20, 112});
  ladders.push_back({650, 192, 20, 112});

  // Row 2 → Row 3  (shift left)
  ladders.push_back({80,  304, 20, 112});
  ladders.push_back({350, 304, 20, 112});
  ladders.push_back({600, 304, 20, 112});

  // Row 3 → Row 4  (shift right)
  ladders.push_back({220, 416, 20, 112});
  ladders.push_back({480, 416, 20, 112});
  ladders.push_back({680, 416, 20, 112});

  // Row 4 → Ground  (shift left)
  ladders.push_back({100, 528, 20, 112});
  ladders.push_back({400, 528, 20, 112});
  ladders.push_back({650, 528, 20, 112});

  // ── Rivets — placed in gaps between ladders on each row
  // Row 4 (platforms[1]): ladders at x=80, 370, 640
  rivets.push_back({220, 528, 30, 16, false, 1});  // gap between x=100 and x=370
  rivets.push_back({510, 528, 30, 16, false, 1});  // gap between x=390 and x=640
  // Row 3 (platforms[2]): ladders at x=180, 450, 680
  rivets.push_back({60,  416, 30, 16, false, 2});  // left of x=180
  rivets.push_back({570, 416, 30, 16, false, 2});  // gap between x=470 and x=680
  // Row 2 (platforms[3]): ladders at x=80, 360, 650
  rivets.push_back({210, 304, 30, 16, false, 3});  // gap between x=100 and x=360
  rivets.push_back({500, 304, 30, 16, false, 3});  // gap between x=380 and x=650
  // Row 1 (platforms[4]): ladders at x=140, 430, 620
  rivets.push_back({50,  192, 30, 16, false, 4});  // left of x=140
  rivets.push_back({540, 192, 30, 16, false, 4});  // gap between x=450 and x=620

  // ── Fire enemies — one per main row
  fire_enemies.emplace_back(300, 528 - Constants::FIRE_H, 40, 720);
  fire_enemies.emplace_back(200, 416 - Constants::FIRE_H, 40, 720);
  fire_enemies.emplace_back(500, 304 - Constants::FIRE_H, 40, 720);
  fire_enemies.emplace_back(150, 192 - Constants::FIRE_H, 40, 720);

  // ── Entity positions — DK center-top, Pauline to his left
  dk_x = W * 0.5f - Constants::DK_W * 0.5f;
  dk_y = 80.0f - Constants::DK_H;
  pauline_x = dk_x - Constants::PAULINE_W - 30.0f;
  pauline_y = 80.0f - Constants::PAULINE_H;
  mario_start_x = W - 60.0f;
  mario_start_y = 640.0f - Constants::MARIO_H;
}

void Level100m::renderBackground(SDL_Renderer* renderer)
{
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderClear(renderer);
}