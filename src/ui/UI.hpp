#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include <string_view>

#include "SDL2/SDL_render.h"
#include "ui/ScoreManager.hpp"

// owns ttf font and draws the hud each frame
class UI
{
 public:
  explicit UI(SDL_Renderer* renderer);
  ~UI();

  // draw score, high score, lives - call after all game objs render
  void renderHUD(const ScoreManager& score, int level_num);

  // Full-screen overlays
  void renderMenu(SDL_Renderer* renderer);
  void renderLevelComplete(SDL_Renderer* renderer, int level_num);
  void renderGameOver(SDL_Renderer* renderer);
  void renderWin(SDL_Renderer* renderer, int final_score);

  bool isReady() const { return font_ != nullptr; }

 private:
  SDL_Renderer* renderer_ = nullptr;
  TTF_Font*     font_ = nullptr;

  void drawText(std::string_view text, int x, int y, SDL_Color color = {255, 255, 255, 255});
};