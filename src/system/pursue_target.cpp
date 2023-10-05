#include "pursue_target.hpp"

#include <entt/entity/registry.hpp>

#include "component/dir.hpp"
#include "component/position.hpp"
#include "component/target.hpp"
#include "pch.hpp"
#include "system/can_move.hpp"
#include "utils/dir.hpp"
#include "utils/pos_coor.hpp"

// ! 不再默认返回上，而是顺时针的下一个方向
Direction getDirectionFromPath(const std::vector<Coor> &path,
                               const Direction nowDir) {
  if (path.size() == 0) {
    return nowDir + 1;
  }
  int offsetX;
  int offsetY;
  if (path.size() == 1) {
    offsetX = -path[0].x;
    offsetY = -path[0].y;
  } else {
    offsetX = path[0].x - path[1].x;
    offsetY = path[0].y - path[1].y;
  }
  if (offsetX == -1) {
    return Direction::Right;
  }
  if (offsetX == 1) {
    return Direction::Left;
  }
  if (offsetY == -1) {
    return Direction::Down;
  }
  if (offsetY == 1) {
    return Direction::Up;
  }
  return nowDir + 1;
}

void PursueTarget(entt::registry &reg, const Maze &maze) {
  constexpr float inf = 1e10f;
  // not include pacman
  auto view = reg.view<Target, Position, MovingDir, IntentionDir>();
  for (const entt::entity e : view) {
    const auto movingDir = reg.get<MovingDir>(e).d;
    const auto &path = view.get<Target>(e).path;
    view.get<IntentionDir>(e).d = getDirectionFromPath(path, movingDir);
  }
}
