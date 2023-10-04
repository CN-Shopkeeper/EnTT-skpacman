#include <entt/entity/registry.hpp>

#include "change_ghost_mode.hpp"
#include "component/ghost.hpp"
#include "component/ghost_mode.hpp"
#include "component/house.hpp"

void ghostScared(entt::registry &reg) {
  const auto view = reg.view<Ghost>();
  for (const entt::entity e : view) {
    reg.remove<ChaseMode, ScatterMode, ScaredMode>(e);
    // Ghosts in EatenMode don't get scared
    if (!reg.all_of<EatenMode>(e)) {
      reg.emplace<ScaredMode>(e);
    }
  }
}

void ghostScaredTimeout(entt::registry &reg) {
  auto view = reg.view<Ghost, ScaredMode>();
  for (const entt::entity e : view) {
    ScaredMode &scared = view.get<ScaredMode>(e);
    --scared.frameToGo;
    if (scared.frameToGo <= 0) {
      // Adding and removing components from the entity that is currently
      // returned by the view is OK
      reg.remove<ScaredMode>(e);
      reg.emplace<ChaseMode>(e);
    }
  }
}

void ghostEaten(entt::registry &reg, const entt::entity ghost) {
  reg.remove<ScaredMode>(ghost);
  reg.emplace<EatenMode>(ghost);
  reg.emplace<EnteringHouse>(ghost);
}

void ghostScatter(entt::registry &reg) {
  const auto view = reg.view<Ghost, ChaseMode>();
  for (const entt::entity e : view) {
    reg.remove<ChaseMode>(e);
    reg.emplace<ScatterMode>(e);
  }
}

void ghostChase(entt::registry &reg) {
  const auto view = reg.view<Ghost, ScatterMode>();
  for (const entt::entity e : view) {
    reg.remove<ScatterMode>(e);
    reg.emplace<ChaseMode>(e);
  }
}
