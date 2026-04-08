#include <iostream>

#include "core/Game.hpp"

int main(int argc, char* argv[])
{
  Game game;

  if (auto result = game.init(); !result)
  {
    std::cerr << "Failed to initialise: " << result.error() << '\n';
    return 1;
  }

  game.run();
  return 0;
}