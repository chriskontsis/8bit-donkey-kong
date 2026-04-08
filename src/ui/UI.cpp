#include "ui/UI.hpp"

#include <format>

#include "SDL2/SDL_render.h"
#include "SDL2/SDL_ttf.h"
#include "ui/ScoreManager.hpp"

UI::UI(SDL_Renderer* renderer) : renderer_(renderer)
{
  if (TTF_Init() == -1)
    return;

  // use macOS sys font
  font_ = TTF_OpenFont("/System/Library/Fonts/Helvetica.ttc", 20);
}

UI::~UI()
{
  if (font_)
    TTF_CloseFont(font_);
  TTF_Quit();
}

void UI::drawText(std::string_view text, int x, int y, SDL_Color color)
{
  if (!font_)
    return;

  SDL_Surface* surf = TTF_RenderText_Solid(font_, text.data(), color);
  if (!surf)
    return;

  SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer_, surf);
  SDL_FreeSurface(surf);
  if (!tex)
    return;

  int w, h;
  SDL_QueryTexture(tex, nullptr, nullptr, &w, &h);
  SDL_Rect dst{x, y, w, h};
  SDL_RenderCopy(renderer_, tex, nullptr, &dst);
  SDL_DestroyTexture(tex);
}

void UI::renderHUD(const ScoreManager& score, int level_num)
{
  drawText(std::format("SCORE: {}", score.score), 560, 10);
  drawText(std::format("HI: {}", score.high_score), 560, 35);
  drawText(std::format("LIVES: {}", score.lives), 560, 60);
  drawText(std::format("LEVEL: {}m", level_num), 560, 85);
}

void UI::renderMenu(SDL_Renderer* renderer)
{
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderClear(renderer);
  drawText("DONKEY KONG", 260, 200, {255, 50, 50, 255});
  drawText("PRESS ENTER TO PLAY", 230, 300, {255, 255, 255, 255});
  drawText("ARROW KEYS: MOVE", 245, 380, {180, 180, 180, 255});
  drawText("SPACE: JUMP", 288, 405, {180, 180, 180, 255});
}

void UI::renderLevelComplete(SDL_Renderer* renderer, int level_num)
{
  drawText("LEVEL COMPLETE!", 250, 280, {255, 220, 50, 255});
  drawText(std::format("NEXT: {}m", level_num), 310, 320, {255, 255, 255, 255});
}

void UI::renderGameOver(SDL_Renderer* renderer)
{
  drawText("GAME OVER", 290, 280, {220, 50, 50, 255});
  drawText("PRESS ENTER", 270, 320, {255, 255, 255, 255});
}

void UI::renderWin(SDL_Renderer* renderer, int final_score)
{
  drawText("YOU RESCUED PAULINE!", 190, 250, {255, 220, 50, 255});
  drawText(std::format("FINAL SCORE: {}", final_score), 230, 300, {255, 255, 255, 255});
  drawText("PRESS ENTER", 270, 340, {180, 180, 180, 255});
}
