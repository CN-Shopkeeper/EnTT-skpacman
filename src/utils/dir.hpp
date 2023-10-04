#pragma once
#include "core/consts.hpp"
#include "pch.hpp"
#include "vector2.hpp"

enum class Direction : std::uint8_t { Right = 0, Down, Left, Up };

constexpr Direction operator+(const Direction &dir, int i) {
  return static_cast<Direction>((static_cast<int>(dir) + i % 4 + 4) % 4);
}

constexpr Direction &operator++(Direction &dir) { return dir = dir + 1; }

constexpr Direction opposite(const Direction dir) { return dir + 2; }

constexpr Direction rotateCW(const Direction dir) { return dir + 1; }

constexpr Direction rotateCCW(const Direction dir) { return dir + -1; }

constexpr Coor DirToCoor(const Direction dir) {
  switch (dir) {
    case Direction::Up:
      return Coor{0, -1};
    case Direction::Down:
      return Coor{0, 1};
    case Direction::Left:
      return Coor{-1, 0};
    case Direction::Right:
    default:
      return Coor{1, 0};
  }
}

constexpr Pos DirToPos(const Direction dir, const int speed) {
  switch (dir) {
    case Direction::Up:
      return Pos{0, -1} * speed;
    case Direction::Down:
      return Pos{0, 1} * speed;
    case Direction::Left:
      return Pos{-1, 0} * speed;
    case Direction::Right:
    default:
      return Pos{1, 0} * speed;
  }
}

// 遍历右下左上四个方向
// for (const Direction dir : dir_range) {
//    ...
// }
struct DirRange {
  constexpr Direction begin() const { return Direction::Right; }

  constexpr Direction end() const { return Direction::Up; }
};
