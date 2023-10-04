#pragma once

#include <entt/entity/fwd.hpp>

struct GhostCollision {
  entt::entity ghost;
  enum class Type {
    none,  // no collision
    eat,   // ghost was eaten
    lost   // player lost a life
  } type;
};

// Deals with the player colliding with ghosts. If the ghost is in scared mode,
// the ghost is said to be eaten. The ID of the eaten ghost is returned so that
// the mode of the ghost can be changed by the change_ghost_mode system.
// If the ghost is in scatter or chase mode, the player will lost a life.
// If the ghost is in eaten mode, the player will pass straight through the
// ghost

GhostCollision PlayerGhostCollide(entt::registry &);