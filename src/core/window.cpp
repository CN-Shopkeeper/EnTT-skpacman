#include "window.hpp"

#include "utils/sdl_check.hpp"

Window::Window(const std::string &title, int w, int h)
    : window(SDL_CHECK(SDL_CreateWindow("Pacman", SDL_WINDOWPOS_CENTERED,
                                        SDL_WINDOWPOS_CENTERED, w, h,
                                        SDL_WINDOW_SHOWN))) {}