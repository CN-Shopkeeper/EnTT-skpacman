#pragma once

#include "pch.hpp"
#include "renderer.hpp"
#include "window.hpp"

class Application {
 public:
  static size_t TileSizeDisplayed;
  Coor pacmanInitCoor;
  Coor ghostInitCoor;

  Application(const std::string& title);
  ~Application();
  void Run();

 private:
  std::unique_ptr<Window> window_;
  std::unique_ptr<Renderer> renderer_;
  std::unique_ptr<TextureManager> textureManager_;

  void startUp();
  void shutDown();
};