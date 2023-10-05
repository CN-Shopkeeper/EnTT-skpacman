#include "pacman_ghost_collide.hpp"

#include <entt/entity/registry.hpp>

#include "component/dir.hpp"
#include "component/ghost.hpp"
#include "component/ghost_mode.hpp"
#include "component/pacman.hpp"
#include "component/pacman_mode.hpp"
#include "component/position.hpp"
#include "utils/dir.hpp"
#include "utils/rect.hpp"

GhostCollision PacmanGhostCollide(entt::registry& reg) {
  const auto pacmans = reg.view<Pacman, Position, MovingDir>();
  const auto ghosts = reg.view<Ghost, Position, MovingDir>();
  for (const entt::entity p : pacmans) {
    const Pos pacmanPos = pacmans.get<Position>(p).p;
    const Direction pacmanDir = pacmans.get<MovingDir>(p).d;
    const Rect pacmanRect = Rect{pacmanPos};
    for (const entt::entity g : ghosts) {
      const Pos ghostPos = ghosts.get<Position>(g).p;
      const Direction ghostDir = ghosts.get<MovingDir>(g).d;

      const Rect ghostRect = Rect{ghostPos};
      if (pacmanRect.IsIntersect(ghostRect)) {
        if (reg.all_of<ScaredMode>(g)) {
          return {g, GhostCollision::Type::eat};
        } else if (reg.all_of<EatenMode>(g)) {
          continue;
        } else {
          if (!(DebugMode || reg.all_of<InvincibleMode>(p))) {
            return {g, GhostCollision::Type::lost};
          }
        }
      }
    }
  }
  return {entt::null, GhostCollision::Type::none};
}