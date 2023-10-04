#pragma once
#include "can_move.hpp"
#include "component/dir.hpp"
#include "core/maze.hpp"
#include "pch.hpp"
#include "utils/vector2.hpp"

class BFSNode : public Vector2i {
 public:
  int step;
  int pre;
  BFSNode(int x, int y, int step, int pre)
      : Vector2i(x, y), step(step), pre(pre) {}
};

std::vector<Coor> ShortestPathBetweenCoors(entt::registry& reg,
                                           const Maze& maze, entt::entity g,
                                           Coor source, Coor target);

void NStepAhead(entt::registry& reg, const Maze& maze, entt::entity pacman,
                int step, Coor& target);

Coor NearestAccessibleTileCoor(entt::registry& reg, const Maze& maze,
                               entt::entity g, Coor target);