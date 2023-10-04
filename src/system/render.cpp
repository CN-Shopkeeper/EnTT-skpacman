#include "render.hpp"

#include "core/app.hpp"

void RenderMap(Maze& maze) {
  auto& renderer = Application::GetInstance().renderer;
  auto tilesheet =
      Application::GetInstance().textureManager->FindTilesheet(TilesheetName);
  for (int y = 0; y < maze.Height(); y++) {
    for (int x = 0; x < maze.Width(); x++) {
      auto tileType = maze.GetTile(x, y);
      renderer->DrawImage(
          tilesheet->Get(static_cast<int>(tileType), 0),
          {x * tilesheet->GetTileSize().w, y * tilesheet->GetTileSize().h});
    }
  }
}