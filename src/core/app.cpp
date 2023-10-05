#include "app.hpp"

#include "consts.hpp"
#include "game.hpp"
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
  window.reset(
      new Window(title, WindowWidth * scaleFactor, WindowHeight * scaleFactor));
  renderer.reset(new Renderer(*window));
  SDL_CHECK(SDL_RenderSetLogicalSize(renderer->renderer.get(), WindowWidth,
                                     WindowHeight));
  textureManager.reset(new TextureManager(renderer.get()));
}

Application::~Application() { SDL_Quit(); }

void Application::StartUp() {
  textureManager->LoadTilesheet(TilesheetName, "./resources/tilesheet.bmp",
                                KeyColor, TilesheetCol, TilesheetRow);
  textureManager->Load("Win", "./resources/win.bmp", KeyColor);
  textureManager->Load("Gameover", "./resources/gameover.bmp", KeyColor);
}

void Application::ShutDown() {}

void Application::Run() {
  GameContext::Init();
  auto& gameCtx = GameContext::GetInstance();
  int frame = 0;
  bool quit = false;
  while (!quit) {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
      if (e.type == SDL_QUIT) {
        quit = true;
        break;
      } else if (e.type == SDL_KEYDOWN) {
        if (e.key.keysym.scancode == SDL_SCANCODE_R) {
          gameCtx.NewGame();
        }
        if (e.key.keysym.scancode == SDL_SCANCODE_P) {
          if (gameCtx.state == GameContext::State::playing) {
            gameCtx.state = GameContext::State::pausing;
          } else if (gameCtx.state == GameContext::State::pausing) {
            gameCtx.state = GameContext::State::playing;
          }
        }
        gameCtx.Input(e.key.keysym.scancode);
      }
    }

    gameCtx.Update();

    renderer->SetColor(SDL_Color{0, 0, 0, 255});
    renderer->Clear();
    gameCtx.Render();
    renderer->Present();
    SDL_Delay(FrameTime);
  }
}
