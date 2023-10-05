#pragma once

#include <entt/entity/fwd.hpp>

#include "core/maze.hpp"
#include "utils/dir.hpp"

bool CanMove(entt::registry &, const Maze &, entt::entity, const Coor coor);