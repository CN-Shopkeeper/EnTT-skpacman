#pragma once
#include <entt/entity/fwd.hpp>

#include "core/maze.hpp"

// Returns the number of Beans that the player ate (usually 0 or 1)
int EatBeans(entt::registry &, Maze &);

// Returns whether the player collided with an PowerBean
bool EatPowerBean(entt::registry &, Maze &);