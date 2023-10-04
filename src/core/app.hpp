#pragma once

#include "pch.hpp"
#include "renderer.hpp"
#include "utils/singlton.hpp"
#include "window.hpp"

class Application : public Singlton<Application> {
 public:
  std::unique_ptr<Window> window;
  std::unique_ptr<Renderer> renderer;
  std::unique_ptr<TextureManager> textureManager;
  static size_t TileSizeDisplayed;
  Coor pacmanInitCoor;
  Coor ghostInitCoor;

  Application(const std::string& title);
  ~Application();
  void Run();

  void StartUp();
  void ShutDown();

 private:
};