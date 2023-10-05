#include "can_move.hpp"

#include <entt/entity/registry.hpp>

#include "component/house.hpp"
#include "utils/pos_coor.hpp"

namespace {
bool isInsideHouse(const Coor coor) {
  if (coor.x >= insideHouseRange[0].x && coor.x <= insideHouseRange[1].x &&
      coor.y >= insideHouseRange[0].y && coor.y <= insideHouseRange[1].y) {
    return true;
  }
  return false;
}
}  // namespace

bool CanMove(entt::registry &reg, const Maze &maze, const entt::entity e,
             const Coor coor) {
  const Tile desiredTile = maze.GetTile(coor);
  const Pos ePos = reg.get<Position>(e).p;
  if (desiredTile == Tile::GhostDoor) {
    if (reg.all_of<EnteringHouse>(e) && !isInsideHouse(PosToCoor(ePos))) {
      return true;
    } else if (reg.all_of<LeavingHouse>(e) && isInsideHouse(PosToCoor(ePos))) {
      return true;
    }
  } else if (desiredTile != Tile::Wall) {
    return true;
  }

  return false;
}