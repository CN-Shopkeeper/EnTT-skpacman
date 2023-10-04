
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

void EnterHouse(entt::registry &reg) {
  const auto view =
      reg.view<EnteringHouse, Position, HomePosition, MovingDir, Movement>();
  for (const entt::entity e : view) {
    const Direction movingDir = view.get<MovingDir>(e).d;
    const Pos pos = view.get<Position>(e).p;
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
  auto outsideHouse0 = GetTheCenter(outsideHouse[0]);
  auto outsideHouse1 = GetTheCenter(outsideHouse[1]);
  for (const entt::entity e : view) {
    if (view.get<Position>(e).p == outsideHouse0 ||
        view.get<Position>(e).p == outsideHouse1) {
      reg.remove<LeavingHouse>(e);
    } else {
      // leaveHouse is called after the set*Target systems so this will
      // overwrite the target position. It's a little bit hacky but it
      // works here.

      // We actually need to do this because ghosts can sometimes get stuck
      // in the house. If their target is towards the bottom of the maze,
      // they will move left and right without going up and out of the house
      auto dis0 = outsideHouse0.ManhattanDistance(view.get<Position>(e).p);
      auto dis1 = outsideHouse1.ManhattanDistance(view.get<Position>(e).p);
      if (dis0 <= dis1) {
        view.get<Target>(e).p = outsideHouse0;
      } else {
        view.get<Target>(e).p = outsideHouse1;
      }
    }
  }
}
