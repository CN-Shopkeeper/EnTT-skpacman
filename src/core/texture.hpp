#pragma once

#include "pch.hpp"
#include "utils/rect.hpp"
#include "utils/sdl_delete.hpp"
#include "utils/vector2.hpp"

class Renderer;

class Texture final {
 public:
  friend class Renderer;
  friend class Image;
  Texture(Renderer* renderer, const std::string& filename,
          const SDL_Color& keyColor, std::optional<size_t> tilesheetIdx);

  const auto& GetSize() const { return size_; }

  bool IsTilesheet() const { return tilesheetIdx_.has_value(); }
  auto GetTilesheetIdx() const { return tilesheetIdx_.value(); }

 private:
  std::optional<size_t> tilesheetIdx_;
  SDL::Texture texture_;

  Renderer* renderer_;
  Size size_;
};

class Image final {
 public:
  friend class Renderer;
  SDL_Color color = {255, 255, 255, 255};

  Image(Texture& texture);
  Image(Texture& texture, Rect rect);

  void SetColorMod(const SDL_Color& _color) { color = _color; }

 private:
  Texture& texture_;
  Rect rect_;
};

class Tilesheet final {
 public:
  Tilesheet(Texture& texture, int col, int row);

  int GetRow() const { return row_; }
  int GetCol() const { return col_; }
  auto& GetTileSize() const { return tileSize_; }

  Image Get(int col, int row) {
    return Image{texture_,
                 Rect({col * tileSize_.w, row * tileSize_.h}, tileSize_)};
  }

 private:
  Texture& texture_;
  int row_;
  int col_;
  Size tileSize_;
};

class TextureManager final {
 public:
  TextureManager(Renderer* renderer) : renderer_(renderer) {}
  TextureManager(const TextureManager&) = delete;
  TextureManager& operator=(const TextureManager&) = delete;

  Texture& Load(const std::string& name, const std::string& filename,
                const SDL_Color& keyColor);
  Texture* Find(const std::string& name);

  Tilesheet& LoadTilesheet(const std::string& name, const std::string& filename,
                           const SDL_Color& keyColor, int col, int row);
  Tilesheet* FindTilesheet(const std::string& name);

 private:
  Renderer* renderer_;
  std::unordered_map<std::string, Texture> textures_;
  std::vector<Tilesheet> tilesheets_;
};