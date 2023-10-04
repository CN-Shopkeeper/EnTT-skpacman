#include "eat_bean.hpp"

#include <entt/entity/registry.hpp>

#include "component/dir.hpp"
#include "component/movement.hpp"
#include "component/pacman.hpp"
#include "component/position.hpp"
#include "utils/pos_coor.hpp"

namespace {

int countConsumptions(entt::registry &reg, Maze &maze, const Tile food) {
  int count = 0;
  const auto view = reg.view<Pacman, Position, MovingDir, Movement>();
  for (const entt::entity e : view) {
    const Pos pos = view.get<Position>(e).p;
    const Coor coor = PosToCoor(pos);
    const Direction movingDir = view.get<MovingDir>(e).d;
    const Movement movement = view.get<Movement>(e);
    if (!maze.IsInside(coor) ||
        // 需要到达中心时才算吃到
        !ReachCenter(reg, e, 0.6f)) {
      continue;
    }
    Tile &tile = maze.GetTile(PosToCoor(pos));
    if (tile == food) {
      ++count;
      tile = Tile::Empty;
    }
  }
  return count;
}

}  // namespace

int EatBeans(entt::registry &reg, Maze &maze) {
  return countConsumptions(reg, maze, Tile::Bean);
}

bool EatPowerBean(entt::registry &reg, Maze &maze) {
  return countConsumptions(reg, maze, Tile::PowerBean);
}
