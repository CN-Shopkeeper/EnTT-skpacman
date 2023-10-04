#pragma once

#include "maze.hpp"
#include "pch.hpp"
#include "utils/singlton.hpp"
class GameContext : public Singlton<GameContext> {
 public:
  GameContext();
  void Input(SDL_Scancode);
  bool Update();
  void Render();

 private:
  enum class State { playing, won, lost };

  entt::registry reg;
  Maze maze;
  int beanCount = 0;
  std::mt19937 rand;
  State state = State::playing;
  int ticks = 0;
};