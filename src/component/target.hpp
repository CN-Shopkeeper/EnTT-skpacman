#pragma ocne
#include "pch.hpp"
#include "utils/vector2.hpp"
struct Target {
  Pos p;
  std::vector<Coor> path;
  Coor checkpoint = {-1, -1};
};