#pragma once

#include "texture_cache.hpp"
#include <containers/indexed_map.hpp>

// Mock TextureCache for testing - doesn't load actual textures
class MockTextureCache : public TextureCache {
 public:
  MockTextureCache() {
    // Create a dummy texture for testing
    Texture2D dummy_texture{};
    dummy_texture.id = 1;  // Non-zero ID to indicate valid texture
    dummy_texture.width = 16;
    dummy_texture.height = 16;
    dummy_texture.mipmaps = 1;
    dummy_texture.format = 7;  // PIXELFORMAT_UNCOMPRESSED_R8G8B8A8

    // Add some mock food textures
    _textures.insert("food_0", dummy_texture);
    _textures.insert("food_1", dummy_texture);
    _textures.insert("food_2", dummy_texture);

    // Add some mock ant textures
    _textures.insert("ants_0", dummy_texture);
    _textures.insert("ants_1", dummy_texture);

    // Set a default texture
    _defaultTextureName = "food_0";
  }

  ~MockTextureCache() {
    // Don't actually unload textures since they weren't really loaded
    _textures.clear();
  }
};

// Global instance for tests
inline MockTextureCache& get_mock_texture_cache() {
  static MockTextureCache cache;
  return cache;
}
