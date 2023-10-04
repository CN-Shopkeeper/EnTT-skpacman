#pragma once

#include "utils/dir.hpp"

//  The direction that the entity wants to go
struct IntentionDir {
  Direction d = Direction::Right;
};

// The direction that the entity is actually going in
struct MovingDir {
  Direction d = Direction::Right;
};
