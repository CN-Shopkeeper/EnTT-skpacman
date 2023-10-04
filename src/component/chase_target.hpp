#pragma once

#include <entt/entity/fwd.hpp>

// These components define the chase targeting behavior for each ghost

// This is where the beauty of ECS comes in
// Blinky doesn't have to chase the pacman!
// The BlinkyChaseTarget component stores the ID of an entity that
// has a Position component. That's it! Blinky can chase anything
// that has a Position. We could quite easily make blinky chase inky

struct BlinkyChaseTarget {
  entt::entity pacman;
};

struct PinkyChaseTarget {
  entt::entity pacman;
};

struct InkyChaseTarget {
  entt::entity pacman;
  entt::entity blinky;
};

struct ClydeChaseTarget {
  entt::entity pacman;
};
