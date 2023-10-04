#pragma once

#include "pch.hpp"
#include "vector2.hpp"

class Tetris final {
 public:
  template <const size_t height, const size_t width>
  static std::array<int, height * width> GenerateTetris() {
    std::array<int, height * width> tetris{0};
    int flag = 1;
    // 鬼门
    tetris[0 + 3 * width] = tetris[1 + 3 * width] = tetris[0 + 4 * width] =
        tetris[1 + 4 * width] = -1;
    // 长生点
    tetris[0 + 7 * width] = -1;
    auto cnt = getTreisCount();
    bfs<height, width>(tetris, 0, 6, cnt, flag++);
    tetris[0 + 7 * width] = 0;
    // 遍历所有的空余地块
    for (int x = 0; x < width; x++) {
      for (int y = 4; y >= 0; y--) {
        if (tetris[x + y * width] == 0) {
          cnt = getTreisCount();
          bfs<height, width>(tetris, x, y, cnt, flag++);
        }
      }
      for (int y = 5; y < height; y++) {
        if (tetris[x + y * width] == 0) {
          cnt = getTreisCount();
          bfs<height, width>(tetris, x, y, cnt, flag++);
        }
      }
    }
    return tetris;
  }
  inline static int randSeed = 0;

 private:
  template <const size_t height, const size_t width>
  static void bfs(std::array<int, height * width>& terris, int x, int y,
                  int cnt, int num) {
    std::mt19937 rng(randSeed);
    std::queue<Coor> queue;
    queue.push({x, y});
    while (cnt >= 0 && !queue.empty()) {
      auto now = queue.front();
      int x = now.x;
      int y = now.y;
      queue.pop();
      terris[now.x + now.y * width] = num;
      cnt--;
      std::vector<Coor> toBeAdded;
      if (x + 1 < width && terris[x + 1 + y * width] == 0) {
        toBeAdded.push_back({x + 1, y});
      }
      if (y - 1 >= 0 && terris[x + (y - 1) * width] == 0) {
        toBeAdded.push_back({x, y - 1});
      }
      if (y + 1 < height && terris[x + (y + 1) * width] == 0) {
        toBeAdded.push_back({x, y + 1});
      }
      std::shuffle(std::begin(toBeAdded), std::end(toBeAdded), rng);
      for (auto v : toBeAdded) {
        queue.push(v);
      }
    }
  }

  static int getTreisCount() {
    std::mt19937 rng(randSeed);  // 手动指定种子
    std::uniform_int_distribution<int> dist(0, 99);
    int rand = dist(rng);
    // 1个的概率: 5%
    // 2个的概率: 10%
    // 3个的概率: 20%
    // 4个的概率: 35%
    // 5个的概率: 30%
    if (rand < 5) {
      return 1;
    } else if (rand < 15) {
      return 2;
    } else if (rand < 35) {
      return 3;
    } else if (rand < 70) {
      return 4;
    } else {
      return 5;
    }
  }
};