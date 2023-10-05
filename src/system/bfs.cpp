#include "bfs.hpp"

#include "component/position.hpp"

std::vector<Coor> ShortestPathBetweenCoors(entt::registry& reg,
                                           const Maze& maze, entt::entity g,
                                           Coor source, Coor target) {
  auto& sTile = maze.GetTile(source);
  auto& tTile = maze.GetTile(target);
  std::vector<Coor> result;
  if (sTile == Tile::Wall || sTile == Tile::GhostDoor || tTile == Tile::Wall ||
      tTile == Tile::GhostDoor) {
    // 起点或终点不可到达，直接返回空向量
    return result;
  }
  std::vector<BFSNode> vec;
  std::array<bool, MapWidth * MapHeight> visited{false};
  vec.push_back({source.x, source.y, 0, -1});
  visited[source.x + source.y * MapWidth] = true;
  int index = 0;
  int size = vec.size();
  while (index != size) {
    auto now = vec[index];
    int x = now.x;
    int y = now.y;
    if (x == target.x && y == target.y) {
      auto& traceBack = now;
      while (traceBack.pre != -1) {
        result.push_back({traceBack.x, traceBack.y});
        traceBack = vec[traceBack.pre];
      }
      result.push_back({traceBack.x, traceBack.y});
      return result;
    }
    // ! 不进行边界检查，因为目前四周边缘都是墙体
    /*
     *  If two or more potential choices are an equal distance from the
     * target, the decision between them is made in the order of up > left >
     * down.
     * 调整四个方向入队顺序（反向的），down > right > up
     */
    if (!visited[x + (y + 1) * MapWidth] && CanMove(reg, maze, g, {x, y + 1})) {
      vec.push_back({x, y + 1, now.step + 1, index});
      visited[x + (y + 1) * MapWidth] = true;
    }
    if (!visited[x + 1 + y * MapWidth] && CanMove(reg, maze, g, {x + 1, y})) {
      vec.push_back({x + 1, y, now.step + 1, index});
      visited[x + 1 + y * MapWidth] = true;
    }
    if (!visited[x + (y - 1) * MapWidth] && CanMove(reg, maze, g, {x, y - 1})) {
      vec.push_back({x, y - 1, now.step + 1, index});
      visited[x + (y - 1) * MapWidth] = true;
    }
    if (!visited[(x - 1) + y * MapWidth] && CanMove(reg, maze, g, {x - 1, y})) {
      vec.push_back({x - 1, y, now.step + 1, index});
      visited[x - 1 + y * MapWidth] = true;
    }

    index++;
    size = vec.size();
  }

  // 不可到达
  return result;
}

void NStepAhead(entt::registry& reg, const Maze& maze, entt::entity pacman,
                int step, Coor& target) {
  const auto pacmanMovingDir = reg.get<MovingDir>(pacman).d;
  const auto pacmanIntentionDir = reg.get<IntentionDir>(pacman).d;
  const auto pacmanMovingCoor = DirToCoor(pacmanMovingDir);
  const auto pacmanIntentionCoor = DirToCoor(pacmanIntentionDir);

  std::queue<BFSNode> queue;
  queue.push({target.x, target.y, 0, 0});
  while (!queue.empty()) {
    auto& now = queue.front();
    queue.pop();
    // 防止只能走到0~3步
    target = {now.x, now.y};
    if (now.step == step) {
      // 走到第step步，直接结束
      break;
    }
    if (CanMove(reg, maze, pacman,
                {now.x + pacmanMovingCoor.x, now.y + pacmanMovingCoor.y})) {
      queue.push({now.x + pacmanMovingCoor.x, now.y + pacmanMovingCoor.y,
                  now.step + 1, 0});
    }
    if (CanMove(
            reg, maze, pacman,
            {now.x + pacmanIntentionCoor.x, now.y + pacmanIntentionCoor.y})) {
      queue.push({now.x + pacmanIntentionCoor.x, now.y + pacmanIntentionCoor.y,
                  now.step + 1, 0});
    }
  }
}

Coor NearestAccessibleTileCoor(entt::registry& reg, const Maze& maze,
                               entt::entity g, Coor target) {
  target.x = std::clamp(target.x, 0, MapWidth - 1);
  target.y = std::clamp(target.y, 0, MapHeight - 1);
  std::queue<BFSNode> queue;
  std::array<bool, MapWidth * MapHeight> visited{false};
  queue.push({target.x, target.y, 0, 0});
  visited[target.x + target.y * MapWidth] = true;
  while (!queue.empty()) {
    auto& now = queue.front();
    queue.pop();
    const auto tile = maze.GetTile(now.x, now.y);
    if (CanMove(reg, maze, g, {now.x, now.y})) {
      // 找到了第一个可以到达的点
      return {now.x, now.y};
    }
    if (maze.IsInside(now.x - 1, now.y) &&
        !visited[now.x - 1 + now.y * MapWidth]) {
      queue.push({now.x - 1, now.y, now.step + 1, now.pre});
      visited[now.x - 1 + now.y * MapWidth] = true;
    }
    if (maze.IsInside(now.x + 1, now.y) &&
        !visited[now.x + 1 + now.y * MapWidth]) {
      queue.push({now.x + 1, now.y, now.step + 1, now.pre});
      visited[now.x + 1 + now.y * MapWidth] = true;
    }
    if (maze.IsInside(now.x, now.y - 1) &&
        !visited[now.x + (now.y - 1) * MapWidth]) {
      queue.push({now.x, now.y - 1, now.step + 1, now.pre});
      visited[now.x + (now.y - 1) * MapWidth] = true;
    }
    if (maze.IsInside(now.x, now.y + 1) &&
        !visited[now.x + (now.y + 1) * MapWidth]) {
      queue.push({now.x, now.y + 1, now.step + 1, now.pre});
      visited[now.x + (now.y + 1) * MapWidth] = true;
    }
  }
  // 找寻不到，应该不会出现这种情况
  return target;
}
