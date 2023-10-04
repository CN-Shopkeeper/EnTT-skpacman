#pragma once

#include <entt/entity/fwd.hpp>

#include "component/movement.hpp"
#include "component/position.hpp"
#include "core/maze.hpp"

// void move(Movement &movement, Position &position, Pos offset);

void Moving(entt::registry &, const Maze &);

void WallCollide(entt::registry &, const Maze &);