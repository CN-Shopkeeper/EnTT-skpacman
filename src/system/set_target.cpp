#include "set_target.hpp"

#include <entt/entity/registry.hpp>

#include "bfs.hpp"
#include "can_move.hpp"
#include "component/chase_target.hpp"
#include "component/dir.hpp"
#include "component/ghost_mode.hpp"
#include "component/home_position.hpp"
#include "component/position.hpp"
#include "component/target.hpp"
#include "utils/dir.hpp"
#include "utils/pos_coor.hpp"

void SetBlinkyChaseTarget(entt::registry &reg, Maze &maze) {
  auto view = reg.view<Target, Position, ChaseMode, BlinkyChaseTarget>();
  for (const entt::entity e : view) {
    const entt::entity pacman = view.get<BlinkyChaseTarget>(e).pacman;
    const auto pacmanPos = reg.get<Position>(pacman).p;
    const auto ghostPos = reg.get<Position>(e).p;
    const auto path = ShortestPathBetweenCoors(
        reg, maze, e, PosToCoor(pacmanPos), PosToCoor(ghostPos));

    auto &target = view.get<Target>(e);
    target.p = pacmanPos;
    target.path = path;
  }
}

void SetPinkyChaseTarget(entt::registry &reg, Maze &maze) {
  auto view = reg.view<Target, ChaseMode, PinkyChaseTarget>();
  for (const entt::entity e : view) {
    const entt::entity pacman = view.get<PinkyChaseTarget>(e).pacman;

    const auto pacmanPos = reg.get<Position>(pacman).p;
    const auto ghostPos = reg.get<Position>(e).p;

    int pacCount = 4;
    auto targetCoor = PosToCoor(pacmanPos);
    NStepAhead(reg, maze, pacman, pacCount, targetCoor);
    const auto path =
        ShortestPathBetweenCoors(reg, maze, e, targetCoor, PosToCoor(ghostPos));

    auto &target = view.get<Target>(e);
    target.p = GetTheCenter(targetCoor);
    target.path = path;
  }
}

void SetInkyChaseTarget(entt::registry &reg, Maze &maze) {
  auto view = reg.view<Target, ChaseMode, InkyChaseTarget>();
  for (const entt::entity e : view) {
    const entt::entity pacman = view.get<InkyChaseTarget>(e).pacman;
    const entt::entity blinky = view.get<InkyChaseTarget>(e).blinky;

    const auto pacmanPos = reg.get<Position>(pacman).p;
    const auto ghostPos = reg.get<Position>(e).p;
    const auto ghostCoor = PosToCoor(ghostPos);

    int pacCount = 4;
    auto targetCoor = PosToCoor(pacmanPos);
    NStepAhead(reg, maze, pacman, pacCount, targetCoor);

    targetCoor = targetCoor * 2 - ghostCoor;
    targetCoor = NearestAccessibleTileCoor(reg, maze, e, targetCoor);
    const auto path =
        ShortestPathBetweenCoors(reg, maze, e, targetCoor, ghostCoor);

    auto &target = view.get<Target>(e);
    target.p = GetTheCenter(targetCoor);
    target.path = path;
  }
}

void SetClydeChaseTarget(entt::registry &reg, Maze &maze) {
  auto view =
      reg.view<Target, Position, ChaseMode, ClydeChaseTarget, HomePosition>();
  for (const entt::entity e : view) {
    const entt::entity pacman = view.get<ClydeChaseTarget>(e).pacman;
    const auto pacmanPos = reg.get<Position>(pacman).p;
    const auto ghostPos = reg.get<Position>(e).p;
    const auto ghostCoor = PosToCoor(ghostPos);
    auto path =
        ShortestPathBetweenCoors(reg, maze, e, PosToCoor(pacmanPos), ghostCoor);

    auto &target = view.get<Target>(e);
    if (path.size() >= 8) {
      target.p = pacmanPos;
      target.path = path;
    } else {
      // ? 是吗
      view.get<Target>(e).p = GetTheCenter(view.get<HomePosition>(e).scatter);
      path = ShortestPathBetweenCoors(
          reg, maze, e, view.get<HomePosition>(e).scatter, ghostCoor);
      target.path = path;
    }
  }
}

void SetScatterTarget(entt::registry &reg, Maze &maze) {
  auto view =
      reg.view<Position, MovingDir, Target, ScatterMode, HomePosition>();
  for (const entt::entity e : view) {
    const Pos pos = view.get<Position>(e).p;
    const Direction movingDir = view.get<MovingDir>(e).d;
    const Coor scatterPoint = view.get<HomePosition>(e).scatter;
    auto &scatterInfo = view.get<ScatterMode>(e);
    if (!scatterInfo.reach && ReachTheTile(reg, e, scatterPoint)) {
      scatterInfo.reach = true;
      const Coor ccwCoor = PosToCoor(pos) + DirToCoor(rotateCCW(movingDir));
      scatterInfo.rotateCCW = CanMove(reg, maze, e, ccwCoor, movingDir);
    }

    auto &target = view.get<Target>(e);
    target.p = GetTheCenter(view.get<HomePosition>(e).scatter);
    if (scatterInfo.reach) {
      const Coor nextRotate = scatterInfo.rotateCCW
                                  ? DirToCoor(rotateCCW(movingDir))
                                  : DirToCoor(rotateCW(movingDir));
      target.path = std::vector<Coor>{nextRotate};
    } else {
      target.path =
          ShortestPathBetweenCoors(reg, maze, e, scatterPoint, PosToCoor(pos));
    }
  }
}

void SetScaredTarget(entt::registry &reg, const Maze &maze,
                     std::mt19937 &rand) {
  auto view = reg.view<Target, Position, ScaredMode, MovingDir>();
  for (const entt::entity e : view) {
    const Pos pos = view.get<Position>(e).p;
    const Direction movingDir = view.get<MovingDir>(e).d;

    const Coor ccwCoor = PosToCoor(pos) + DirToCoor(rotateCCW(movingDir));
    const Coor cwCoor = PosToCoor(pos) + DirToCoor(rotateCW(movingDir));

    Coor nextStep;
    std::uniform_int_distribution<int> dist(0, 7);
    int randNum = dist(rand);
    if (randNum < 3) {
      if (CanMove(reg, maze, e, ccwCoor, movingDir)) {
        nextStep = DirToCoor(rotateCCW(movingDir));
      } else {
        nextStep = DirToCoor(rotateCW(movingDir));
      }
    } else if (randNum < 6) {
      if (CanMove(reg, maze, e, cwCoor, movingDir)) {
        nextStep = DirToCoor(rotateCCW(movingDir));
      } else {
        nextStep = DirToCoor(rotateCCW(movingDir));
      }
    } else {
      nextStep = DirToCoor(movingDir);
    }

    // 设置为自己
    view.get<Target>(e).p = pos;
    view.get<Target>(e).path = std::vector<Coor>{nextStep};
  }
}

void SetEatenTarget(entt::registry &reg, Maze &maze) {
  auto view = reg.view<Position, Target, EatenMode, HomePosition>();
  for (const entt::entity e : view) {
    const Pos pos = view.get<Position>(e).p;
    view.get<Target>(e).p = GetTheCenter(view.get<HomePosition>(e).home);
    view.get<Target>(e).path = ShortestPathBetweenCoors(
        reg, maze, e, view.get<HomePosition>(e).home, PosToCoor(pos));
  }
}
