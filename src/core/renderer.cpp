#include "renderer.hpp"

#include "utils/sdl_check.hpp"

Renderer::Renderer(const Window& window)
    : renderer(SDL_CHECK(SDL_CreateRenderer(
          window.window.get(), -1,
          SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC))) {}

void Renderer::SetColor(const SDL_Color& color) {
  SDL_SetRenderDrawColor(renderer.get(), color.r, color.g, color.b, color.a);
}

void Renderer::Clear() { SDL_RenderClear(renderer.get()); }

void Renderer::Present() { SDL_RenderPresent(renderer.get()); }

void Renderer::DrawTexture(Texture& texture, const SDL_Rect& rect, int x,
                           int y) {
  SDL_Rect dst = {x, y, rect.w, rect.h};
  SDL_RenderCopy(renderer.get(), texture.texture_.get(), &rect, &dst);
}

void Renderer::DrawImage(const Image& image, const Pos& position,
                         const std::optional<Size>& size ) {
  SDL_FRect dst = {position.x, position.y, size ? size->w : image.rect_.w,
                   size ? size->h : image.rect_.h};
  SDL_Rect src = {
      static_cast<int>(image.rect_.x), static_cast<int>(image.rect_.y),
      static_cast<int>(image.rect_.w), static_cast<int>(image.rect_.h)};
  SDL_RenderCopyF(renderer.get(), image.texture_.texture_.get(), &src, &dst);
}

void Renderer::DrawImage(const Image& image, const Pos& position,
                         const Size scale, float rotation) {
  SDL_FRect dst = {position.x, position.y, std::abs(scale.x) * image.rect_.w,
                   std::abs(scale.y) * image.rect_.h};
  SDL_Rect src = {
      static_cast<int>(image.rect_.x), static_cast<int>(image.rect_.y),
      static_cast<int>(image.rect_.w), static_cast<int>(image.rect_.h)};

  uint32_t flip = SDL_RendererFlip::SDL_FLIP_NONE;
  flip |= scale.x < 0 ? SDL_RendererFlip::SDL_FLIP_HORIZONTAL : 0;
  flip |= scale.y < 0 ? SDL_RendererFlip::SDL_FLIP_VERTICAL : 0;
  auto texture = image.texture_.texture_.get();
  SDL_SetTextureColorMod(texture, image.color.r, image.color.g, image.color.b);
  SDL_RenderCopyExF(renderer.get(), image.texture_.texture_.get(), &src, &dst,
                    rotation, nullptr, static_cast<SDL_RendererFlip>(flip));
}
void Renderer::DrawPath(const std::unique_ptr<SDL_Point[]>& path,
                        const SDL_Color& color, int count) {
  SDL_SetRenderDrawColor(renderer.get(), color.r, color.g, color.b, color.a);
  SDL_RenderDrawLines(renderer.get(), path.get(), count);
}