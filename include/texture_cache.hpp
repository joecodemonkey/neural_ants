#pragma once

#include <raylib.h>

#include <string>
#include <unordered_map>

class TextureCache {
 public:
  auto add_texture(const std::string& name, const std::string& path) -> bool;
  auto has_texture(const std::string& name) const -> bool;
  auto get_texture(const std::string& name) -> Texture2D&;
  auto set_default(const std::string& name) -> bool;
  auto get_default(const std::string& name) -> Texture2D&;

  ~TextureCache();

  TextureCache() = default;
  TextureCache(TextureCache const&) = delete;
  TextureCache& operator=(TextureCache const&) = delete;

 protected:
  std::unordered_map<std::string, Texture2D> _textures;
  std::string _defaultTextureName;
};