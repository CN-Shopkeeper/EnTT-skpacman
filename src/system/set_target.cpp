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

namespace {
bool shouldSetTarget(entt::registry &reg, entt::entity e, const Maze &maze) {
  if (!reg.all_of<Target, Position, MovingDir, IntentionDir>(e)) {
    return false;
  }
  const Pos pos = reg.get<Position>(e).p;
  const Direction movingDir = reg.get<MovingDir>(e).d;
  const Direction intentionDir = reg.get<IntentionDir>(e).d;
  Target &target = reg.get<Target>(e);
  Coor coor = PosToCoor(pos);
  if (target.checkpoint == coor) {
    auto directionCor = DirToCoor(intentionDir);
    if (!CanMove(reg, maze, e, coor + directionCor)) {
      auto forward = DirToCoor(movingDir) + coor;
      auto ccwDir = rotateCCW(movingDir);
      auto cwDir = rotateCW(movingDir);
      if (CanMove(reg, maze, e, forward)) {
        target.path = std::vector<Coor>{DirToCoor(movingDir)};
      } else if (CanMove(reg, maze, e, coor + DirToCoor(ccwDir))) {
        target.path = std::vector<Coor>{DirToCoor(ccwDir)};
      } else if (CanMove(reg, maze, e, coor + DirToCoor(cwDir))) {
        target.path = std::vector<Coor>{DirToCoor(cwDir)};
      } else {
        target.path = std::vector<Coor>{DirToCoor(opposite(movingDir))};
      }
    }
    return false;
  }
  const Coor ccwCoor = coor + DirToCoor(rotateCCW(movingDir));
  const Coor cwCoor = coor + DirToCoor(rotateCW(movingDir));
  if (!CanMove(reg, maze, e, ccwCoor) && !CanMove(reg, maze, e, cwCoor)) {
    return false;
  }
  target.checkpoint = coor;
  return true;
}
}  // namespace

void SetBlinkyChaseTarget(entt::registry &reg, entt::entity e,
                          const Maze &maze) {
  if (!shouldSetTarget(reg, e, maze)) {
    return;
  }
  const entt::entity pacman = reg.get<BlinkyChaseTarget>(e).pacman;
  const auto pacmanPos = reg.get<Position>(pacman).p;
  const auto ghostPos = reg.get<Position>(e).p;
  const auto path = ShortestPathBetweenCoors(reg, maze, e, PosToCoor(pacmanPos),
                                             PosToCoor(ghostPos));

  auto &target = reg.get<Target>(e);
  target.path = path;
}

void SetPinkyChaseTarget(entt::registry &reg, entt::entity e,
                         const Maze &maze) {
  if (!shouldSetTarget(reg, e, maze)) {
    return;
  }
  const entt::entity pacman = reg.get<PinkyChaseTarget>(e).pacman;

  const auto pacmanPos = reg.get<Position>(pacman).p;
  const auto ghostPos = reg.get<Position>(e).p;

  int pacCount = 4;
  auto targetCoor = PosToCoor(pacmanPos);
  NStepAhead(reg, maze, pacman, pacCount, targetCoor);
  const auto path =
      ShortestPathBetweenCoors(reg, maze, e, targetCoor, PosToCoor(ghostPos));

  auto &target = reg.get<Target>(e);
  target.path = path;
}

void SetInkyChaseTarget(entt::registry &reg, entt::entity e, const Maze &maze) {
  if (!shouldSetTarget(reg, e, maze)) {
    return;
  }
  const entt::entity pacman = reg.get<InkyChaseTarget>(e).pacman;
  const entt::entity blinky = reg.get<InkyChaseTarget>(e).blinky;

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

  auto &target = reg.get<Target>(e);
  target.path = path;
}

void SetClydeChaseTarget(entt::registry &reg, entt::entity e,
                         const Maze &maze) {
  if (!shouldSetTarget(reg, e, maze)) {
    return;
  }
  const entt::entity pacman = reg.get<ClydeChaseTarget>(e).pacman;
  const auto pacmanPos = reg.get<Position>(pacman).p;
  const auto ghostPos = reg.get<Position>(e).p;
  const auto ghostCoor = PosToCoor(ghostPos);
  auto path =
      ShortestPathBetweenCoors(reg, maze, e, PosToCoor(pacmanPos), ghostCoor);

  auto &target = reg.get<Target>(e);
  if (path.size() >= 8) {
    target.path = path;
  } else {
    // ? 是吗
    path = ShortestPathBetweenCoors(
        reg, maze, e, reg.get<HomePosition>(e).scatter, ghostCoor);
    target.path = path;
  }
}

void SetScatterTarget(entt::registry &reg, entt::entity e, const Maze &maze) {
  if (!shouldSetTarget(reg, e, maze)) {
    return;
  }
  const Pos pos = reg.get<Position>(e).p;
  const Direction movingDir = reg.get<MovingDir>(e).d;
  const Coor scatterPoint = reg.get<HomePosition>(e).scatter;
  auto &scatterInfo = reg.get<ScatterMode>(e);

  if (!scatterInfo.reach && ReachTheTile(reg, e, scatterPoint)) {
    scatterInfo.reach = true;
    const Coor ccwCoor = PosToCoor(pos) + DirToCoor(rotateCCW(movingDir));
    scatterInfo.rotateCCW = CanMove(reg, maze, e, ccwCoor);
  }

  auto &target = reg.get<Target>(e);
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

void SetScaredTarget(entt::registry &reg, entt::entity e, const Maze &maze,
                     std::mt19937 &rand) {
  if (!shouldSetTarget(reg, e, maze)) {
    return;
  }
  const Pos pos = reg.get<Position>(e).p;
  const Direction movingDir = reg.get<MovingDir>(e).d;

  const Coor ccwCoor = PosToCoor(pos) + DirToCoor(rotateCCW(movingDir));
  const Coor cwCoor = PosToCoor(pos) + DirToCoor(rotateCW(movingDir));

  Coor nextStep;
  std::uniform_int_distribution<int> dist(0, 7);
  int randNum = dist(rand);
  if (randNum < 3) {
    if (CanMove(reg, maze, e, ccwCoor)) {
      nextStep = DirToCoor(rotateCCW(movingDir));
    } else {
      nextStep = DirToCoor(rotateCW(movingDir));
    }
  } else if (randNum < 6) {
    if (CanMove(reg, maze, e, cwCoor)) {
      nextStep = DirToCoor(rotateCCW(movingDir));
    } else {
      nextStep = DirToCoor(rotateCCW(movingDir));
    }
  } else {
    nextStep = DirToCoor(movingDir);
  }

  reg.get<Target>(e).path = std::vector<Coor>{nextStep};
}

void SetEatenTarget(entt::registry &reg, entt::entity e, const Maze &maze) {
  if (!shouldSetTarget(reg, e, maze)) {
    return;
  }
  const Pos pos = reg.get<Position>(e).p;
  reg.get<Target>(e).path = ShortestPathBetweenCoors(
      reg, maze, e, reg.get<HomePosition>(e).home, PosToCoor(pos));
}

void SetTarget(entt::registry &reg, const Maze &maze, std::mt19937 &rand) {
  auto view = reg.view<Target, MovingDir, Position, HomePosition>();
  for (const entt::entity e : view) {
    if (reg.all_of<ChaseMode>(e)) {
      if (reg.all_of<BlinkyChaseTarget>(e)) {
        SetBlinkyChaseTarget(reg, e, maze);
      }
      if (reg.all_of<PinkyChaseTarget>(e)) {
        SetPinkyChaseTarget(reg, e, maze);
      }
      if (reg.all_of<InkyChaseTarget>(e)) {
        SetInkyChaseTarget(reg, e, maze);
      }
      if (reg.all_of<ClydeChaseTarget>(e)) {
        SetClydeChaseTarget(reg, e, maze);
      }
    } else if (reg.all_of<ScatterMode>(e)) {
      SetScatterTarget(reg, e, maze);
    } else if (reg.all_of<ScaredMode>(e)) {
      SetScaredTarget(reg, e, maze, rand);
    } else if (reg.all_of<EatenMode>(e)) {
      SetEatenTarget(reg, e, maze);
    }
  }
}