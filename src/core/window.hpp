#pragma once

#include "utils/sdl_delete.hpp"
class Window final {
 public:
  SDL::Window window;
  Window(const std::string& title, int w, int h);

 private:
};