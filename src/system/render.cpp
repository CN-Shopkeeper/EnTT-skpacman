#include "render.hpp"

#include "component/dir.hpp"
#include "component/ghost_mode.hpp"
#include "component/pacman_mode.hpp"
#include "component/position.hpp"
#include "component/sprite.hpp"
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

void RenderPacman(entt::registry& reg) {
  const auto view = reg.view<Position, MovingDir, IntentionDir, PacmanSprite>();
  for (const entt::entity e : view) {
    const Pos pos = view.get<Position>(e).p;
    const Direction movingDir = view.get<MovingDir>(e).d;
    // todo choose image
    const Image iamge = view.get<PacmanSprite>(e).images[0];
    Size scale;
    float rotation = 0;
    scale.x = movingDir == Direction::Left ? -1 : 1;
    scale.y = 1;
    if (movingDir == Direction::Up) {
      rotation = -90;
    }
    if (movingDir == Direction::Down) {
      rotation = 90;
    }

    auto& renderer = Application::GetInstance().renderer;
    renderer->DrawImage(iamge, pos, scale, rotation);
  }
}

void RenderGhost(entt::registry& reg) {
  const auto view = reg.view<Position, MovingDir, IntentionDir, GhostSprite>();
  for (const entt::entity e : view) {
    const Pos pos = view.get<Position>(e).p;
    const Direction movingDir = view.get<MovingDir>(e).d;
    GhostSprite& sprite = view.get<GhostSprite>(e);
    Size scale;
    float rotation = 0;
    scale.x = movingDir == Direction::Left ? -1 : 1;
    scale.y = 1;
    if (movingDir == Direction::Up) {
      rotation = -90;
    }
    if (movingDir == Direction::Down) {
      rotation = 90;
    }
    
    if (reg.all_of<ScaredMode>(e)) {
      sprite.image.color = ScaredColor;
    } else if (reg.all_of<EatenMode>(e)) {
      sprite.image.color = WhiteColor;
    } else {
      sprite.image.color = sprite.color;
    }
    auto& renderer = Application::GetInstance().renderer;
    renderer->DrawImage(sprite.image, pos, scale, rotation);
  }
}
