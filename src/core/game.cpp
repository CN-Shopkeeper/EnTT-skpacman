#include "game.hpp"

#include "app.hpp"
#include "factory.hpp"
#include "system/bfs.hpp"
#include "system/change_monster_mode.hpp"
#include "system/eat_bean.hpp"
#include "system/house.hpp"
#include "system/movement.hpp"
#include "system/pacman_ghost_collide.hpp"
#include "system/player_input.hpp"
#include "system/pursue_target.hpp"
#include "system/render.hpp"
#include "system/set_target.hpp"

GameContext::GameContext()
    : maze(Maze{Maze::GenerateMap(beanCount), {MapWidth, MapHeight}}) {
  auto tilesheet =
      Application::GetInstance().textureManager->FindTilesheet(TilesheetName);
  const entt::entity pacman = MakePacman(
      reg, {tilesheet->Get(static_cast<int>(ImageTileType::Pacman), 0),
            tilesheet->Get(static_cast<int>(ImageTileType::PacmanEat), 0)});
  blinky = MakeBlinky(
      reg, tilesheet->Get(static_cast<int>(ImageTileType::Ghost), 0), pacman);
  pinky = MakePinky(
      reg, tilesheet->Get(static_cast<int>(ImageTileType::Ghost), 0), pacman);
  inky =
      MakeInky(reg, tilesheet->Get(static_cast<int>(ImageTileType::Ghost), 0),
               pacman, blinky);
  clyde = MakeClyde(
      reg, tilesheet->Get(static_cast<int>(ImageTileType::Ghost), 0), pacman);

  JoinChasing(reg, blinky);
  JoinChasing(reg, pinky);
  // seeding a pseudo random number generator with a random source
  rand.seed(std::random_device{}());
}

void GameContext::Input(const SDL_Scancode key) {
  if (state == State::playing) {
    PlayerInput(reg, key);
  }
}

bool GameContext::Update() {
  globalFrame++;
  if (state != State::playing) {
    return true;
  }
  PacmanInvincibleTimeout(reg);
  if (energizedFrame > 0) {
    GhostScaredTimeout(reg);
    energizedFrame--;
    if (energizedFrame == 0) {
      multiKillReward = MultiKillReward;
    }
  } else {
    gameFrame++;
    auto epoch = GetElapsedGameTime() % 27;
    // 根据计时器更改ghost的状态
    if (epoch < 7) {
      GhostScatter(reg);
    } else {
      GhostChase(reg);
    }
  }

  Moving(reg, maze);
  WallCollide(reg, maze);
  score -= beanEaten * 10;
  beanEaten += EatBeans(reg, maze);
  if (EatPowerBean(reg, maze)) {
    beanEaten++;
    energizedFrame = ghostScaredTime * Framerate;
    GhostScared(reg);
  }

  score += beanEaten * 10;

  if (beanEaten == 30) {
    // 吃掉超过30个豆子时，inky加入战斗
    JoinChasing(reg, inky);
  }
  if (beanEaten == beanCount / 3) {
    // 吃掉超过1/3豆子时，clyde加入战斗
    JoinChasing(reg, clyde);
  }

  EnterHouse(reg);
  SetTarget(reg, maze, rand);
  LeaveHouse(reg);
  PursueTarget(reg, maze);

  const GhostCollision collision = PacmanGhostCollide(reg);
  if (collision.type == GhostCollision::Type::eat) {
    GhostEaten(reg, collision.ghost);
    score += multiKillReward;
    multiKillReward += MultiKillReward;
  }
  if (collision.type == GhostCollision::Type::lost) {
    if (lifeRemains <= 0) {
      state = State::lost;
    } else {
      lifeRemains--;
      PacmanInvincible(reg);
    }
  } else if (beanEaten == beanCount) {
    state = State::won;
  }

  if (globalFrame % 4 == 0) {
    std::cout << "Score: " << score << std::endl;
  }
  return true;
}

void GameContext::Render() {
  RenderMap(maze);
  RenderPacman(reg, globalFrame);
  RenderGhost(reg, globalFrame );
  if (state == State::won) {
    auto& renderer = Application::GetInstance().renderer;
    renderer->DrawTexture(
        *Application::GetInstance().textureManager->Find("Win"),
        SDL_Rect{0, 0, 256, 256}, (WindowWidth - 256) / 2,
        (WindowHeight - 256) / 2);
  } else if (state == State::lost) {
    auto& renderer = Application::GetInstance().renderer;
    renderer->DrawTexture(
        *Application::GetInstance().textureManager->Find("Gameover"),
        SDL_Rect{0, 0, 256, 256}, (WindowWidth - 256) / 2,
        (WindowHeight - 256) / 2);
  }
}
