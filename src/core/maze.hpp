#pragma once

#include "utils/matrix.hpp"
#include "utils/tile.hpp"
#include "utils/vector2.hpp"

class Maze final {
 public:
  Maze(std::string_view desc, const Size& size);
  static std::string GenerateMap(int& beanCount);

  const Tile& GetTile(int x, int y) const { return tiles_->Get(x, y); }
  const Tile& GetTile(Coor cor) const { return tiles_->Get(cor.x, cor.y); }
  Tile& GetTile(Coor cor) { return tiles_->Get(cor.x, cor.y); }
  int Width() const { return tiles_->Width(); }
  int Height() const { return tiles_->Height(); }

  void Draw();

  // 坐标是否在地图内
  bool IsInside(int x, int y) const {
    return x >= 0 && x < Width() && y >= 0 && y < Height();
  }

  bool IsInside(Coor cor) const { return IsInside(cor.x, cor.y); }

 private:
  std::unique_ptr<Matrix<Tile>> tiles_;
};