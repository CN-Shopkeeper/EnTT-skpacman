#pragma once

#include <entt/entity/fwd.hpp>

// This is called when pacman eats an energizer. All ghosts in chase or scatter
// mode will be put into scared mode. Ghosts that are already in scared mode
// will have there scared timer extended
// ?refreshed?
void GhostScared(entt::registry &);

// Ghosts in scared mode have a timer. When the timer runs out, they will enter
// chase mode.
void GhostScaredTimeout(entt::registry &);

// This is called when pacman collides with a ghost that is in scared mode.
// This system puts the ghost into eaten mode
void GhostEaten(entt::registry &, entt::entity);

// Puts all of the ghosts into scatter mode
void GhostScatter(entt::registry &);

// Puts all of the ghost into chase mode
void GhostChase(entt::registry &);

void PacmanInvincibleTimeout(entt::registry &);
void PacmanInvincible(entt::registry &reg);
