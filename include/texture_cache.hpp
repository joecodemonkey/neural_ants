#pragma once

#include <raylib.h>

#include <containers/indexed_map.hpp>
#include <string>
#include <vector>

class TextureCache {
 public:
  auto add_texture(const std::string& name, const std::string& path) -> bool;
  auto has_texture(const std::string& name) const -> bool;
  auto get_texture(const std::string& name) -> Texture2D&;
  auto set_default(const std::string& name) -> bool;
  auto get_default(const std::string& name) -> Texture2D&;

  // Dynamic loading - extracts prefix from folder path automatically
  auto load_textures(const std::string& folder_path) -> bool;
  [[nodiscard]] auto get_texture_keys(const std::string& prefix) const
      -> std::vector<std::string>;

  // Index-based access for efficient rendering
  auto get_texture(size_t index) -> Texture2D&;
  auto get_random_texture_index(const std::string& prefix) const -> size_t;

  ~TextureCache();

  TextureCache() = default;
  TextureCache(TextureCache const&) = delete;
  TextureCache& operator=(TextureCache const&) = delete;

 protected:
  IndexedMap<Texture2D> _textures;
  std::string _defaultTextureName;
};