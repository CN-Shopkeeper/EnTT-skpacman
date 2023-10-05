#include "change_monster_mode.hpp"

#include <entt/entity/registry.hpp>

#include "component/ghost.hpp"
#include "component/ghost_mode.hpp"
#include "component/house.hpp"
#include "component/pacman.hpp"
#include "component/pacman_mode.hpp"

void PacmanInvincible(entt::registry &reg) {
  auto view = reg.view<Pacman>();

  for (const entt::entity e : view) {
    reg.remove<InvincibleMode>(e);
    reg.emplace<InvincibleMode>(e);
  }
}

void PacmanInvincibleTimeout(entt::registry &reg) {
  auto view = reg.view<Pacman, InvincibleMode>();
  for (const entt::entity e : view) {
    InvincibleMode &invincible = view.get<InvincibleMode>(e);
    --invincible.frameToGo;
    if (invincible.frameToGo <= 0) {
      reg.remove<InvincibleMode>(e);
    }
  }
}

void GhostScared(entt::registry &reg) {
  const auto view = reg.view<Ghost>();
  for (const entt::entity e : view) {
    reg.remove<ChaseMode, ScatterMode, ScaredMode>(e);
    // Ghosts in EatenMode don't get scared
    if (!reg.all_of<EatenMode>(e)) {
      reg.emplace<ScaredMode>(e);
    }
  }
}

void GhostScaredTimeout(entt::registry &reg) {
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

void GhostEaten(entt::registry &reg, const entt::entity ghost) {
  reg.remove<ScaredMode>(ghost);
  reg.emplace<EatenMode>(ghost);
  reg.emplace<EnteringHouse>(ghost);
}

void GhostScatter(entt::registry &reg) {
  const auto view = reg.view<Ghost, ChaseMode>();
  for (const entt::entity e : view) {
    reg.remove<ChaseMode>(e);
    reg.emplace<ScatterMode>(e);
  }
}

void GhostChase(entt::registry &reg) {
  const auto view = reg.view<Ghost, ScatterMode>();
  for (const entt::entity e : view) {
    reg.remove<ScatterMode>(e);
    reg.emplace<ChaseMode>(e);
  }
}
