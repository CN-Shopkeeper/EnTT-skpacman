#include "can_move.hpp"

#include <entt/entity/registry.hpp>

#include "component/house.hpp"
#include "utils/pos_coor.hpp"

bool CanMove(entt::registry &reg, const Maze &maze, const entt::entity e,
             const Coor coor, const Direction movingDir) {
  const Tile desiredTile = maze.GetTile(coor);
  if (desiredTile == Tile::GhostDoor) {
    if (reg.all_of<EnteringHouse>(e) && movingDir == Direction::Down) {
      return true;
    } else if (reg.all_of<LeavingHouse>(e) && movingDir == Direction::Up) {
      return true;
    }
  } else if (desiredTile != Tile::Wall) {
    return true;
  }

  return false;
}