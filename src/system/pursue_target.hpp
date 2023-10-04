#pragma once

#include <entt/entity/fwd.hpp>

#include "core/maze.hpp"

// This system reads the target position from the Target component and pursues
// it by setting the DesiredDir component. Pacman doesn't use A*. Ghosts will
// turn in the direction that brings them closer to the player

void PursueTarget(entt::registry &, const Maze &);