#include <entt/entt.hpp>
#include <iostream>

#include "core/app.hpp"

int main(int argc, char** argv) {
  entt::registry registry;
  // 在这里编写您的EnTT代码

  try {
    Application app("entt_pacman");
    app.Run();
  } catch (std::exception& e) {
    // The only exceptions we should get are from SDL
    std::cout << e.what() << '\n';
    return 1;
  }
  return 0;
}