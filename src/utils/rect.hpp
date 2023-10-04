#pragma once

#include "core/consts.hpp"
#include "pch.hpp"
#include "vector2.hpp"

class Rect final {
 public:
  union {
    SDL_FRect sdlrect;
    struct {
      Pos position;
      Size size;
    } _;
    struct {
      float x, y, w, h;
    };
  };

  Rect(const Pos& pos, const Size& size = {TileSize, TileSize}) {
    this->_.position = pos;
    this->_.size = size;
  }

  bool IsIntersect(const Rect& rect) const {
    return !(_.position.x + _.size.x <= rect._.position.x ||
             _.position.x >= rect._.position.x + rect._.size.x ||
             _.position.y + _.size.y <= rect._.position.y ||
             _.position.y >= rect._.position.y + rect._.size.y);
  }

  Pos Center() const { return _.position + _.size / 2.0; }
};