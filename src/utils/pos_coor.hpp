#pragma once

#include "component/dir.hpp"
#include "component/movement.hpp"
#include "component/position.hpp"
#include "core/consts.hpp"
#include "dir.hpp"
#include "pch.hpp"
#include "rect.hpp"
#include "vector2.hpp"

inline Coor PosToCoor(const Pos pos, int tileSize = TileSize) {
  return {static_cast<int>(pos.x / tileSize),
          static_cast<int>(pos.y / tileSize)};
}

inline Pos GetTheCenter(const Coor coor, int tileSize = TileSize) {
  return Rect{{coor.x * tileSize * 1.f, coor.y * tileSize * 1.f},
              {tileSize * 1.f, tileSize * 1.f}}
      .Center();
}

inline Pos GetTheAnchor(const Coor coor, int tileSize = TileSize) {
  return {coor.x * tileSize * 1.f, coor.y * tileSize * 1.f};
}

inline bool ReachCenter(entt::registry &reg, entt::entity e,
                        float threshold = 0.5f, float offset = 0.0f,
                        int tileSize = TileSize) {
  const Pos pos = reg.get<Position>(e).p;
  const Direction movingDir = reg.get<MovingDir>(e).d;
  const float speed = reg.get<Movement>(e).speed;
  auto posCenter = Rect{pos, {tileSize * 1.f, tileSize * 1.f}}.Center();
  auto tileCenter = GetTheCenter(PosToCoor(pos));
  auto diff = tileCenter - posCenter;
  bool reach = false;
  // 1.5是Right=0，Down=1和Left=2，Up=3的分界线
  const double offset_ =
      1.5 - static_cast<int>(movingDir) > 0 ? -offset : offset;
  if (movingDir == Direction::Left || movingDir == Direction::Right) {
    if (diff.x >= speed * (offset_ - threshold) &&
        diff.x <= speed * (offset_ + threshold)) {
      reach = true;
    }
  }
  if (movingDir == Direction::Up || movingDir == Direction::Down) {
    if (diff.y >= speed * (offset_ - threshold) &&
        diff.y <= speed * (offset_ + threshold)) {
      reach = true;
    }
  }
  return reach;
}

inline bool ReachTheTile(entt::registry &reg, entt::entity e,
                         const Coor targetCoor, float threshold = 0.5f,
                         float offset = 0.0f, int tileSize = TileSize) {
  const Pos pos = reg.get<Position>(e).p;
  return PosToCoor(pos) == targetCoor &&
         ReachCenter(reg, e, threshold, offset, tileSize);
}
