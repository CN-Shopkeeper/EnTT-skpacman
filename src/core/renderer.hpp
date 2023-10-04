#pragma once

#include "texture.hpp"
#include "utils/sdl_delete.hpp"
#include "utils/vector2.hpp"
#include "window.hpp"

class Renderer final {
 public:
  SDL::Renderer renderer;
  Renderer(const Window&);

  void SetColor(const SDL_Color& color);
  void Clear();
  void Present();
  void DrawTexture(Texture& texture, const SDL_Rect& rect, int x, int y);
  void DrawImage(const Image& image, const Pos& position,
                 const std::optional<Size>& size = std::nullopt);
  void DrawImage(const Image& image, const Pos& position, const Size scale,
                 float rotation = 0);
  void DrawPath(const std::unique_ptr<SDL_Point[]>& path,
                const SDL_Color& color, int count);

 private:
};