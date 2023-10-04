#pragma once

#include "utils/matrix.hpp"
#include "utils/tile.hpp"
#include "utils/vector2.hpp"

class Maze final {
 public:
  std::unique_ptr<Matrix<Tile>> tiles;

  Maze(std::string_view desc, const Size& size);
  static std::string GenerateMap(int& beanCount,
                                 std::optional<int> seed = std::nullopt);

  const Tile& GetTile(int x, int y) const { return tiles->Get(x, y); }
  const Tile& GetTile(Coor cor) const { return tiles->Get(cor.x, cor.y); }
  Tile& GetTile(Coor cor) { return tiles->Get(cor.x, cor.y); }
  int Width() const { return tiles->Width(); }
  int Height() const { return tiles->Height(); }

  // 坐标是否在地图内
  bool IsInside(int x, int y) const {
    return x >= 0 && x < Width() && y >= 0 && y < Height();
  }

  bool IsInside(Coor cor) const { return IsInside(cor.x, cor.y); }

 private:
};