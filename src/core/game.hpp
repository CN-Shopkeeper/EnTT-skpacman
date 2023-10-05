#pragma once

#include "maze.hpp"
#include "pch.hpp"
#include "texture.hpp"
#include "utils/singlton.hpp"
class GameContext : public Singlton<GameContext> {
 public:
  enum class State { playing, won, lost, pausing };
  State state = State::playing;
  GameContext();
  void Input(SDL_Scancode);
  bool Update();
  void Render();
  void NewGame();
  int GetElapsedGameTime() const {
    return static_cast<int>(std::floor(gameFrame * FrameTime / 1000));
  }

 private:
  entt::entity pacman;
  entt::entity blinky;
  entt::entity pinky;
  entt::entity inky;
  entt::entity clyde;

  entt::registry reg;
  int beanCount = 0;
  Maze maze;
  int beanEaten = 0;
  int lifeRemains = 3;
  std::mt19937 rand;
  int globalFrame = 0;
  int gameFrame = 0;
  int energizedFrame = 0;
  int multiKillReward = MultiKillReward;
  int score = 0;
};