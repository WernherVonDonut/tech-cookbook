#include "texture.hpp"

#include <iostream>

namespace sdl_lazyfoo
{
namespace lesson10
{

Texture::Texture(SDL_Renderer* renderer) : renderer_(renderer)
{
  texture_ = nullptr;
  width_ = 0;
  height_ = 0;
}

Texture::~Texture()
{
  DeallocateTexture();
  if (texture_) {
    delete texture_;
  }
}

bool Texture::LoadFromFile(const std::string& path)
{
  DeallocateTexture();
  SDL_Texture* texture = nullptr;
  SDL_Surface* loaded_surface = IMG_Load(path.c_str());
  if (loaded_surface == nullptr) {
    std::cerr << "Unable to load image for path: " << path << " Error: " << IMG_GetError() << std::endl;
  } else {
    // This means which pixel color determines the background transparancy on the image, our lad is white, so it is
    // white
    SDL_SetColorKey(loaded_surface, SDL_TRUE, SDL_MapRGB(loaded_surface->format, 0xFF, 0xFF, 0xFF));
    texture = SDL_CreateTextureFromSurface(renderer_, loaded_surface);
    if (texture == nullptr) {
      std::cerr << "Unable to create texture for path: " << path << " Error: " << IMG_GetError() << std::endl;
    } else {
      width_ = loaded_surface->w;
      height_ = loaded_surface->h;
    }
    SDL_FreeSurface(loaded_surface);
  }
  texture_ = texture;
  return texture_ != nullptr;
}

void Texture::DeallocateTexture()
{
  if (texture_ != nullptr) {
    SDL_DestroyTexture(texture_);
    texture_ = nullptr;
    width_ = 0;
    height_ = 0;
  }
}

void Texture::Render(const int x, const int y)
{
  SDL_Rect renderQuad{x, y, width_, height_};
  SDL_RenderCopy(renderer_, texture_, nullptr, &renderQuad);
}

int Texture::GetWidth() const { return width_; };
int Texture::GetHeight() const { return height_; };

} // namespace lesson10

} // namespace sdl_lazyfoo