#pragma once

#include "core/maze.hpp"
#include "pch.hpp"

// These systems calculate the target position of ghosts.
// Each ghost has a different target depending on which mode it's in.

// There is a chase target system for each ghost because each ghost
// behaves differently in chase mode.
void SetBlinkyChaseTarget(entt::registry &, Maze &maze);
void SetPinkyChaseTarget(entt::registry &, Maze &maze);
void SetInkyChaseTarget(entt::registry &, Maze &maze);
void SetClydeChaseTarget(entt::registry &, Maze &maze);

// Each ghost has a "home corner" which is stored in the HomePosition component
// Ghosts will simply move towards their home corner and end up going round
// in circles
void SetScatterTarget(entt::registry &, Maze &maze);

// Ghosts move randomly in scared mode so we pass in a std::mt19937 for
// random numbers
void SetScaredTarget(entt::registry &, const Maze &, std::mt19937 &);

// When a ghost is eaten, their eyes will return the ghost house to be revived
void SetEatenTarget(entt::registry &, Maze &maze);