#include "movement.hpp"

#include <entt/entity/registry.hpp>

#include "component/dir.hpp"
#include "component/ghost.hpp"
#include "component/ghost_mode.hpp"
#include "component/target.hpp"
#include "system/can_move.hpp"
#include "utils/dir.hpp"
#include "utils/pos_coor.hpp"

void move(Movement &movement, Position &position, Pos offset) {
  movement.offset = offset;
  position.p += offset;
}

void setSpeed(entt::registry &reg, entt::entity e) {
  if (reg.all_of<ScaredMode>(e)) {
    reg.get<Movement>(e).speed = 2.f;
  } else if (reg.all_of<EatenMode>(e)) {
    reg.get<Movement>(e).speed = 8.f;
  } else {
    reg.get<Movement>(e).speed = 4.f;
  }
}

void Moving(entt::registry &reg, const Maze &maze) {
  auto view = reg.view<Position, MovingDir, IntentionDir, Movement>();
  for (const entt::entity e : view) {
    if (reg.all_of<Ghost>(e) && !reg.all_of<Target>(e)) {
      continue;
    }
    setSpeed(reg, e);
    Position &position = view.get<Position>(e);
    Direction &movingDir = view.get<MovingDir>(e).d;
    const Direction intentionDir = view.get<IntentionDir>(e).d;
    Movement &movement = view.get<Movement>(e);

    if (intentionDir == movingDir) {
      move(movement, position, DirToPos(movingDir, movement.speed));
    } else if (opposite(movingDir) == intentionDir) {
      movingDir = intentionDir;
      move(movement, position, DirToPos(movingDir, movement.speed));
    } else {
      auto posCenter = Rect{position.p}.Center();
      auto tileCoor = PosToCoor(position.p);
      auto tileX = tileCoor.x;
      auto tileY = tileCoor.y;
      auto tileCenter = GetTheCenter(tileCoor);
      auto diff = tileCenter - posCenter;
      auto reach = ReachCenter(reg, e, 1.0f, 0.4f);
      bool should = false;

      if (reach) {
        const int dirOffset = 1.5 - static_cast<int>(intentionDir) > 0 ? 1 : -1;
        if (movingDir == Direction::Left || movingDir == Direction::Right) {
          if (maze.IsInside(tileX, tileY + dirOffset)) {
            should = CanMove(reg, maze, e, {tileX, (tileY + dirOffset)});
          }
        }
        if (movingDir == Direction::Up || movingDir == Direction::Down) {
          if (maze.IsInside(tileX + dirOffset, tileY)) {
            should = CanMove(reg, maze, e, {(tileX + dirOffset), tileY});
          }
        }
      }

      if (!should) {
        move(movement, position, DirToPos(movingDir, movement.speed));
      } else {
        move(movement, position, diff);
        movingDir = intentionDir;
      }
    }
  }
}

void WallCollide(entt::registry &reg, const Maze &maze) {
  auto view = reg.view<Position, MovingDir, IntentionDir, Movement>();
  for (const entt::entity e : view) {
    Position &position = view.get<Position>(e);
    const Direction intentionDir = view.get<IntentionDir>(e).d;
    auto &movement = view.get<Movement>(e);
    const auto velocity = movement.offset;
    auto posRect = Rect{position.p};
    auto posCenter = posRect.Center();
    int cx = posCenter.x / TileSize;
    int cy = posCenter.y / TileSize;

    for (int dx = -1; dx <= 1; dx++) {
      for (int dy = -1; dy <= 1; dy++) {
        auto x = dx + cx, y = dy + cy;
        if (!maze.IsInside(x, y)) {
          continue;
        }
        Rect tileRect(Pos(x * TileSize, y * TileSize));
        if (maze.GetTile(x, y) == Tile::Wall && tileRect.IsIntersect(posRect)) {
          posCenter = posRect.Center();
          auto tileCenter = tileRect.Center();
          auto dir = posCenter - tileCenter;
          auto maxIntersectV = (posRect._.size + tileRect._.size) / 2.0;
          auto intersectDepth =
              maxIntersectV - Vector2(std::abs(dir.x), std ::abs(dir.y));
          auto seperate = (velocity * -1).Normalize<float>() * intersectDepth;
          move(movement, position, seperate);
          posRect = Rect(position.p);
        }
      }
    }
  }
}