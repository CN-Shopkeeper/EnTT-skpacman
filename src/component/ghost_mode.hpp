#include "core/consts.hpp"

struct ChaseMode {};
struct ScatterMode {
  bool reach = false;
  bool rotateCCW = false;
};
struct ScaredMode {
  // This frameToGo counts down the number of ticks that the ghost is scared
  // for. Once the frameToGo reaches zero, the ghost enters chase mode. This is
  // handled by the change_ghost_mode systems
  int frameToGo = ghostScaredTime * Framerate;
};
// EatenMode可以与ChaseMode和ScatterMode同时存在
struct EatenMode {};