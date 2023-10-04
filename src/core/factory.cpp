#include "factory.hpp"

#include "component/chase_target.hpp"
#include "component/dir.hpp"
#include "component/ghost.hpp"
#include "component/ghost_mode.hpp"
#include "component/home_position.hpp"
#include "component/house.hpp"
#include "component/movement.hpp"
#include "component/pacman.hpp"
#include "component/position.hpp"
#include "component/sprite.hpp"
#include "component/target.hpp"
#include "utils/pos_coor.hpp"

entt::entity MakePacman(entt::registry &reg, std::array<Image, 2> images) {
  const entt::entity e = reg.create();
  reg.emplace<Pacman>(e, 3);
  reg.emplace<MovingDir>(e, Direction::Right);
  reg.emplace<IntentionDir>(e, Direction::Right);
  reg.emplace<Position>(e, GetTheAnchor(PacmanInitCoor));
  reg.emplace<Movement>(e, 5.0f, Pos{0, 0});
  reg.emplace<PacmanSprite>(e, images);
  return e;
}

namespace {

entt::entity makeGhost(entt::registry &reg, const Coor home, const Coor scatter,
                       Image image, const SDL_Color color) {
  const entt::entity e = reg.create();
  reg.emplace<Position>(e, GetTheAnchor(home));
  reg.emplace<HomePosition>(e, home, scatter);
  reg.emplace<ScatterMode>(e);
  reg.emplace<IntentionDir>(e);
  reg.emplace<MovingDir>(e);
  reg.emplace<Target>(e);
  reg.emplace<Ghost>(e);
  reg.emplace<GhostSprite>(e, image, color);
  return e;
}

}  // namespace

entt::entity MakeBlinky(entt::registry &reg, Image image, entt::entity pacman) {
  const entt::entity e = makeGhost(reg, GhostInitCoor + Coor{2, 0},
                                   Coor{MapWidth - 2, 1}, image, BlinkyColor);
  reg.emplace<BlinkyChaseTarget>(e, pacman);
  return e;
}

entt::entity MakePinky(entt::registry &reg, Image image, entt::entity pacman) {
  const entt::entity e =
      makeGhost(reg, GhostInitCoor + Coor{1, 0}, Coor{1, 1}, image, PinkyColor);
  reg.emplace<PinkyChaseTarget>(e, pacman);
  return e;
}

entt::entity MakeInky(entt::registry &reg, Image image, entt::entity pacman,
                      entt::entity blinky) {
  const entt::entity e = makeGhost(
      reg, GhostInitCoor, {MapWidth - 2, MapHeight - 2}, image, InkyColor);
  reg.emplace<InkyChaseTarget>(e, pacman, blinky);
  return e;
}

entt::entity MakeClyde(entt::registry &reg, Image image, entt::entity pacman) {
  const entt::entity e = makeGhost(reg, GhostInitCoor + Coor{3, 0},
                                   {1, MapHeight - 2}, image, ClydeColor);
  reg.emplace<PinkyChaseTarget>(e, pacman);
  return e;
}