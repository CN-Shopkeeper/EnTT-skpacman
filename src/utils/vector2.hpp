#pragma once

#include "pch.hpp"

template <typename T, typename = std::enable_if_t<std::is_arithmetic_v<T>>>
class Vector2 {
 public:
  union {
    T data[2];
    struct {
      T x, y;
    };
    struct {
      T w, h;
    };
  };

  constexpr Vector2() : x(0), y(0) {}

  constexpr Vector2(T x, T y) : x(x), y(y) {}

  constexpr explicit Vector2(T value) : x(value), y(value) {}

  void Set(T x, T y) {
    this->x = x;
    this->y = y;
  }

  constexpr Vector2 operator+(const Vector2& o) const {
    return Vector2{o.x + x, o.y + y};
  }

  constexpr Vector2 operator-(const Vector2& o) const {
    return Vector2{x - o.x, y - o.y};
  }

  constexpr Vector2 operator*(const Vector2& o) const {
    return Vector2{o.x * x, o.y * y};
  }

  constexpr Vector2 operator/(const Vector2& o) const {
    return Vector2{x / o.x, y / o.y};
  }

  constexpr Vector2 operator*(T value) const {
    return Vector2{x * value, y * value};
  }

  constexpr Vector2 operator/(T value) const {
    return Vector2{x / value, y / value};
  }

  constexpr Vector2& operator+=(const Vector2& o) {
    *this = *this + o;
    return *this;
  }

  constexpr Vector2& operator-=(const Vector2& o) {
    *this = *this - o;
    return *this;
  }

  constexpr Vector2& operator*=(const Vector2& o) {
    *this = *this * o;
    return *this;
  }

  constexpr Vector2& operator/=(const Vector2& o) {
    *this = *this / o;
    return *this;
  }

  constexpr bool operator==(const Vector2 rhs) {
    return x == rhs.x && y == rhs.y;
  }

  constexpr T LengthSqrd() const { return Dot(*this); }

  template <typename U, typename = std::enable_if_t<std::is_arithmetic_v<U>>>
  constexpr U Length() const {
    return std::sqrt(static_cast<U>(LengthSqrd()));
  }

  constexpr T Dot(const Vector2& o) const { return x * o.x + y * o.y; }

  inline T ManhattanDistance(const Vector2<T> o) {
    return std::abs(x - o.x) + std::abs(y - o.y);
  }

  template <typename U, typename = std::enable_if_t<std::is_arithmetic_v<U>>>
  Vector2<U> Normalize() const {
    U len = Length<U>();
    if (std::abs(len) <= std::numeric_limits<U>::epsilon()) {
      return *this;
    }
    return *this / Length<U>();
  }
};

using Vector2i = Vector2<int>;
using Vector2f = Vector2<float>;

using Pos = Vector2f;
using Coor = Vector2i;
using Size = Vector2f;
