#include <iostream>
#include <texture_cache.hpp>

bool TextureCache::add_texture(const std::string& name, const std::string& path) {
  if (has_texture(name)) {
    return false;
  }

  Texture2D texture = LoadTexture(path.c_str());

  if (texture.id == 0) {
    return false;
  }

  // Add texture to cache
  _textures[name] = texture;

  if (_textures.size() == 1) {
    _defaultTextureName = name;
  }

  return true;
}

bool TextureCache::has_texture(const std::string& name) const {
  return _textures.find(name) != _textures.end();
}

Texture2D TextureCache::get_texture(const std::string& name) {
  auto it = _textures.find(name);
  if (it != _textures.end()) {
    return it->second;
  }

  if (!_defaultTextureName.empty()) {
    return get_texture(_defaultTextureName);
  }

  throw std::runtime_error("Texture '" + name + "' not found and no default texture is set");
}

bool TextureCache::set_default(const std::string& name) {
  if (!has_texture(name)) {
    return false;
  }

  _defaultTextureName = name;
  return true;
}

TextureCache::~TextureCache() {
  for (auto& pair : _textures) {
    UnloadTexture(pair.second);
  }
  _textures.clear();
}