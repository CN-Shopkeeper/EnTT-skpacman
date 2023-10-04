#include "player_input.hpp"

#include <entt/entity/registry.hpp>

#include "component/dir.hpp"
#include "component/pacman.hpp"
#include "system/can_move.hpp"

namespace {

std::optional<Direction> readDir(const SDL_Scancode key) {
  switch (key) {
    case SDL_SCANCODE_W:
    case SDL_SCANCODE_UP:
      return Direction::Up;
    case SDL_SCANCODE_D:
    case SDL_SCANCODE_RIGHT:
      return Direction::Right;
    case SDL_SCANCODE_S:
    case SDL_SCANCODE_DOWN:
      return Direction::Down;
    case SDL_SCANCODE_A:
    case SDL_SCANCODE_LEFT:
      return Direction::Left;
    default:
      return std::nullopt;
  }
}

}  // namespace

bool playerInput(entt::registry &reg, const SDL_Scancode key) {
  const auto dirOpt = readDir(key);
  if (!dirOpt.has_value()) {
    return false;
  }
  const auto dir = dirOpt.value();
  auto view = reg.view<Pacman, IntentionDir>();
  for (const entt::entity e : view) {
    view.get<IntentionDir>(e).d = dir;
  }
  return true;
}
