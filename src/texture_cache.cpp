#include <iostream>
#include <texture_cache.hpp>

auto TextureCache::add_texture(const std::string& name, const std::string& path) -> bool {
  if (has_texture(name)) {
    return false;
  }

  Texture2D texture = LoadTexture(path.c_str());

  if (texture.id == 0) {
    return false;
  }

  _textures[name] = texture;

  if (_textures.size() == 1) {
    _defaultTextureName = name;
  }

  return true;
}

auto TextureCache::has_texture(const std::string& name) const -> bool {
  return _textures.find(name) != _textures.end();
}

auto TextureCache::get_texture(const std::string& name) -> Texture2D& {
  auto it = _textures.find(name);
  if (it != _textures.end()) {
    return it->second;
  }

  if (!_defaultTextureName.empty()) {
    return get_texture(_defaultTextureName);
  }

  throw std::runtime_error("Texture '" + name + "' not found and no default texture is set");
}

auto TextureCache::set_default(const std::string& name) -> bool {
  if (!has_texture(name)) {
    return false;
  }

  _defaultTextureName = name;
  return true;
}

TextureCache::~TextureCache() {
  for (auto& pair : _textures) {
    std::cerr << "Unloading Texture\n";
    /* TODO: FIX THIS */
    // UnloadTexture(pair.second);
  }
  _textures.clear();
}