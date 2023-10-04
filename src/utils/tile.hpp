#pragma once

#include "core/consts.hpp"

enum class Tile : int {
  Empty = -1,
  Wall = static_cast<int>(ImageTileType::Wall),
  Bean = static_cast<int>(ImageTileType::Bean),
  PowerBean = static_cast<int>(ImageTileType::PowerBean),
  GhostDoor = static_cast<int>(ImageTileType::GhostDoor)
};