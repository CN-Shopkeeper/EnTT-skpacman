#pragma once

#include <entt/entity/fwd.hpp>

// These systems deal with ghosts entering and leaving the house

void EnterHouse(entt::registry &);
void LeaveHouse(entt::registry &);