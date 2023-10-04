#include "maze.hpp"

#include "pch.hpp"
#include "utils/tetris.hpp"

Maze::Maze(std::string_view desc, const Size& size) {
  tiles.reset(
      new Matrix<Tile>(static_cast<int>(size.w), static_cast<int>(size.h)));
  for (int y = 0; y < size.h; y++) {
    for (int x = 0; x < size.x; x++) {
      char c = desc[x + y * size.w];
      Tile tile;
      switch (c) {
        case 'W':
          tile = Tile::Wall;
          break;
        case 'B':
          tile = Tile::Bean;
          break;
        case 'P':
          tile = Tile::PowerBean;
          break;
        case ' ':
          tile = Tile::Empty;
          break;
        case 'G':
          tile = Tile::GhostDoor;
          break;
        default:
          break;
      }
      tiles->Get(x, y) = tile;
    }
  }
}

std::string Maze::GenerateMap(int& beanCount, std::optional<int> seed) {
  if (seed != std::nullopt) {
    Tetris::randSeed = seed.value();
  }
  auto tetris = Tetris::GenerateTetris<tetrisHeight, tetrisWidth>();
  const size_t width3 = tetrisWidth * 3;
  const size_t height3 = tetrisHeight * 3;
  std::array<int, height3 * width3> terris3{0};
  std::array<int, height3 * width3> halfPath{0};
  std::array<int, MapWidth * MapHeight> finalPath{0};

  for (int y = 0; y < tetrisHeight; y++) {
    for (int x = 0; x < tetrisWidth; x++) {
      for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
          terris3[(x * 3 + i) + (y * 3 + j) * tetrisWidth * 3] =
              tetris[x + y * tetrisWidth];
        }
      }
    }
  }
  // 右边或者上面或右上不一样
  for (int i = 1; i < width3 - 1; i++) {
    for (int j = 1; j < height3 - 1; j++) {
      if (terris3[i + j * (width3)] != terris3[i + (j - 1) * (width3)] ||
          terris3[i + j * (width3)] != terris3[(i + 1) + (j) * (width3)] ||
          terris3[i + j * (width3)] != terris3[(i + 1) + (j - 1) * (width3)]) {
        halfPath[i + j * (width3)] = 1;
      }
    }
  }
  // 设置鬼门
  halfPath[1 + (3 * 3 + 1) * width3] = 2;
  // 鬼门内置空
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      halfPath[1 + i + (3 * 3 + 2 + j) * width3] = -1;
    }
  }
  // 出生点附近置空
  halfPath[1 + (3 * 7) * width3] = halfPath[2 + (3 * 7) * width3] = -1;
  for (int y = 0; y < height3; y++) {
    halfPath[0 + y * width3] = halfPath[1 + y * width3];
    halfPath[width3 - 1 + y * width3] = 1;
  }
  for (int x = 0; x < width3; x++) {
    halfPath[x] = 1;
    halfPath[x + (height3 - 1) * width3] = 1;
  }
  for (int y = 0; y < height3; y++) {
    for (int x = 1; x < width3; x++) {
      finalPath[width3 + x - 1 + (y + 1) * width3 * 2] =
          finalPath[width3 - x + (y + 1) * width3 * 2] =
              halfPath[x + y * width3];
    }
  }

  beanCount = 0;
  std::string mapStr = std::string(MapWidth * MapHeight, ' ');
  // 这一步暂不生成能量豆，在下一步生成
  for (int y = 0; y < MapHeight; y++) {
    for (int x = 0; x < MapWidth; x++) {
      switch (finalPath[x + y * MapWidth]) {
        case 0:
          mapStr[x + y * MapWidth] = 'W';
          break;
        case 1:
          mapStr[x + y * MapWidth] = 'B';
          beanCount++;
          break;
        case 2:
          mapStr[x + y * MapWidth] = 'G';
          break;
        case -1:
          mapStr[x + y * MapWidth] = ' ';
          break;
        default:
          break;
      }
    }
  }
  auto interval = beanCount / 5 + 1;
  auto tempCount = 0;
  for (int i = 0; i < mapStr.length(); i++) {
    if (mapStr[i] == 'B') {
      tempCount++;
      if (tempCount == interval) {
        mapStr[i] = 'P';
        tempCount = 0;
      }
    }
  }
  return mapStr;
}
