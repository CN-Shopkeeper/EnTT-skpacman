#pragma once

#include "pch.hpp"

namespace SDL {

struct DeleteWindow {
  void operator()(SDL_Window *ptr) const noexcept { SDL_DestroyWindow(ptr); }
};

struct DeleteTexture {
  void operator()(SDL_Texture *ptr) const noexcept { SDL_DestroyTexture(ptr); }
};

struct DeleteSurface {
  void operator()(SDL_Surface *ptr) const noexcept { SDL_FreeSurface(ptr); }
};

struct DeleteRenderer {
  void operator()(SDL_Renderer *ptr) const noexcept {
    SDL_DestroyRenderer(ptr);
  }
};

using Window = std::unique_ptr<SDL_Window, DeleteWindow>;
using Texture = std::unique_ptr<SDL_Texture, DeleteTexture>;
using Surface = std::unique_ptr<SDL_Surface, DeleteSurface>;
using Renderer = std::unique_ptr<SDL_Renderer, DeleteRenderer>;

}  // namespace SDL
