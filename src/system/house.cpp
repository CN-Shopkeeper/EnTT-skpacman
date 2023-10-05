
#include "house.hpp"

#include <entt/entity/registry.hpp>

#include "component/dir.hpp"
#include "component/ghost_mode.hpp"
#include "component/home_position.hpp"
#include "component/house.hpp"
#include "component/movement.hpp"
#include "component/position.hpp"
#include "component/target.hpp"
#include "core/consts.hpp"
#include "utils/pos_coor.hpp"

namespace {
bool isOutsideHouse(const Coor coor) {
  if (coor.x >= outsideHouseRange[0].x && coor.x <= outsideHouseRange[1].x &&
      coor.y >= outsideHouseRange[0].y && coor.y <= outsideHouseRange[1].y) {
    return false;
  }
  return true;
}
}  // namespace

void EnterHouse(entt::registry &reg) {
  const auto view =
      reg.view<EnteringHouse, Position, HomePosition, MovingDir, Movement>();
  for (const entt::entity e : view) {
    const float speed = view.get<Movement>(e).speed;
    if (ReachTheTile(reg, e, view.get<HomePosition>(e).home)) {
      reg.remove<EnteringHouse>(e);
      reg.emplace<LeavingHouse>(e);
      reg.remove<EatenMode>(e);
      reg.emplace<ChaseMode>(e);
    }
  }
}

void LeaveHouse(entt::registry &reg) {
  auto view = reg.view<LeavingHouse, Position, Target>();
  for (const entt::entity e : view) {
    if (isOutsideHouse(PosToCoor(view.get<Position>(e).p))) {
      reg.remove<LeavingHouse>(e);
    }
  }
}

void JoinChasing(entt::registry &reg, entt::entity e) {
  reg.remove<LeavingHouse, Target>(e);
  reg.emplace<LeavingHouse>(e);
  reg.emplace<Target>(e);
}
