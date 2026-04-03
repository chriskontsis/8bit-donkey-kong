#pragma once

#include <SDL2/SDL.h>

#include <array>

class InputHandler
{
 public:
  InputHandler();

  // call once at top of every frame, before game logic
  void update();

  bool isHeld(SDL_Scancode key) const;
  bool justPressed(SDL_Scancode key) const;
  bool quitRequested() const { return quit_; }

 private:
  static constexpr int         KEY_COUNT = SDL_NUM_SCANCODES;
  std::array<Uint8, KEY_COUNT> current_{};
  std::array<Uint8, KEY_COUNT> previous_{};
  bool                         quit_ = false;
};