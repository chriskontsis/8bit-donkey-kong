#include "InputHandler.hpp"

#include <cstring>

#include "SDL2/SDL_events.h"
#include "SDL2/SDL_keyboard.h"
#include "SDL2/SDL_scancode.h"
#include "SDL2/SDL_stdinc.h"

InputHandler::InputHandler()
{
  current_.fill(0);
  previous_.fill(0);
}

void InputHandler::update()
{
  // drain sdl event queue for key events & for sdl getkbstate to stay updated
  SDL_Event event;
  while (SDL_PollEvent(&event))
  {
    if (event.type == SDL_QUIT)
    {
      quit_ = true;
    }
    if (event.type == SDL_KEYDOWN && event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
    {
      quit_ = true;
    }
  }

  // snapshot to save current -> previous & read fresh state
  previous_ = current_;
  const Uint8* live = SDL_GetKeyboardState(nullptr);
  std::memcpy(current_.data(), live, KEY_COUNT);
}

bool InputHandler::isHeld(SDL_Scancode key) const
{
  return current_[key] != 0;
}

bool InputHandler::justPressed(SDL_Scancode key) const
{
  return current_[key] != 0 && previous_[key] == 0;
}