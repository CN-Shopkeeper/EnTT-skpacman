#include "app.hpp"

#include "consts.hpp"
#include "pch.hpp"
#include "utils/sdl_check.hpp"
#include "utils/sdl_delete.hpp"

namespace {

int getScaleFactor() {
  // Make the largest window possible with an integer scale factor
  SDL_Rect bounds;
#if SDL_VERSION_ATLEAST(2, 0, 5)
  SDL_CHECK(SDL_GetDisplayUsableBounds(0, &bounds));
#else
#warning SDL 2.0.5 or later is recommended
  SDL_CHECK(SDL_GetDisplayBounds(0, &bounds));
#endif
  const int scaleX = bounds.w / WindowWidth;
  const int scaleY = bounds.h / WindowHeight;
  return std::max(1, std::min(scaleX, scaleY));
}
}  // namespace

Application::Application(const std::string& title) {
  SDL_CHECK(SDL_Init(SDL_INIT_EVERYTHING));
  const int scaleFactor = getScaleFactor();
  std::cout << "Using scale factor: " << scaleFactor << '\n';
  window_.reset(
      new Window(title, WindowWidth * scaleFactor, WindowHeight * scaleFactor));
  renderer_.reset(new Renderer(*window_));
  SDL_CHECK(SDL_RenderSetLogicalSize(renderer_->renderer.get(), WindowWidth,
                                     WindowHeight));
  textureManager_.reset(new TextureManager(renderer_.get()));
}

Application::~Application() { SDL_Quit(); }

void Application::startUp() {}

void Application::shutDown() {}

void Application::Run() {
    int frame = 0;
  bool quit = false;
  while (!quit) {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
      if (e.type == SDL_QUIT) {
        quit = true;
        break;
      } else if (e.type == SDL_KEYDOWN) {
        // game.input(e.key.keysym.scancode);
      }
      SDL_Delay(30);
    }

    // Game::logic is called once for each tile
    // Game::render is called for each pixel between tiles
    // if (frame % tileSize == 0) {
    //   if (!game.logic()) {
    //     quit = true;
    //   }
    // }

    // SDL_CHECK(SDL_SetRenderDrawColor(renderer.get(), 0, 0, 0, 255));
    // SDL_CHECK(SDL_RenderClear(renderer.get()));
    // game.render(writer, frame % tileSize);
    // ++frame;
    // SDL_RenderPresent(renderer.get());
  }
}
