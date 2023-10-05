#include "game.hpp"

#include "app.hpp"
#include "component/house.hpp"
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
  const entt::entity blinky = MakeBlinky(
      reg, tilesheet->Get(static_cast<int>(ImageTileType::Ghost), 0), pacman);
  const entt::entity pinky = MakePinky(
      reg, tilesheet->Get(static_cast<int>(ImageTileType::Ghost), 0), pacman);
  MakeInky(reg, tilesheet->Get(static_cast<int>(ImageTileType::Ghost), 0),
           pacman, blinky);
  MakeClyde(reg, tilesheet->Get(static_cast<int>(ImageTileType::Ghost), 0),
            pacman);

  reg.emplace<LeavingHouse>(blinky);
  reg.emplace<LeavingHouse>(pinky);
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
  beanEaten += EatBeans(reg, maze);
  if (EatPowerBean(reg, maze)) {
    beanEaten++;
    energizedFrame = ghostScaredTime * Framerate;
    GhostScared(reg);
  }
  EnterHouse(reg);
  SetTarget(reg, maze, rand);
  LeaveHouse(reg);
  PursueTarget(reg, maze);

  const GhostCollision collision = PacmanGhostCollide(reg);
  if (collision.type == GhostCollision::Type::eat) {
    GhostEaten(reg, collision.ghost);
  }
  if (collision.type == GhostCollision::Type::lost) {
    state = State::lost;
  } else if (beanEaten == beanCount) {
    state = State::won;
  }
  return true;
}

void GameContext::Render() {
  RenderMap(maze);
  RenderPacman(reg, globalFrame % 16 < 8 ? 0 : 1);
  RenderGhost(reg);
  // if (state == State::won) {
  //   fullRender(writer, animera::SpriteID::win);
  // } else if (state == State::lost) {
  //   fullRender(writer, animera::SpriteID::lose);
  // }
}
