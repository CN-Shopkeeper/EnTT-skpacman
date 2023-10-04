#pragma once

#include "core/texture.hpp"
#include "pch.hpp"

struct GhostSprite {
  Image image;
  SDL_Color color;
};

struct PacmanSprite {
  std::array<Image, 2> images;
};