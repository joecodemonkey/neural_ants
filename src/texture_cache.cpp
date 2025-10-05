#include <filesystem>
#include <iostream>
#include <ranges>
#include <texture_cache.hpp>
#include <util/file.hpp>

auto TextureCache::add_texture(const std::string& name, const std::string& path) -> bool {
  if (has_texture(name)) {
    return false;
  }

  Texture2D texture = LoadTexture(path.c_str());

  if (texture.id == 0) {
    std::cerr << "LoadTexture failed for: " << path << " (texture.id == 0)\n";
    return false;
  }

  if (!IsTextureValid(texture)) {
    std::cerr << "Invalid texture loaded from: " << path << "\n";
    UnloadTexture(texture);
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

auto TextureCache::load_textures(const std::string& folder_path) -> bool {
  // Extract prefix from folder path (last directory name)
  std::filesystem::path path(folder_path);
  std::string prefix = path.filename().string() + "_";

  // Get all .png files from the folder
  auto files_result = Util::File::get_files(folder_path, ".png");

  if (!files_result) {
    std::cerr << "Failed to load textures from " << folder_path << ": " << files_result.error()
              << "\n";
    return false;
  }

  if (files_result->empty()) {
    std::cerr << "No PNG files found in " << folder_path << "\n";
    return false;
  }

  // Load each texture
  size_t loaded_count = 0;
  for (const auto& file_info : *files_result) {
    // Remove .png extension to get base name
    std::string base_name = file_info.name.substr(0, file_info.name.find_last_of('.'));

    // Create texture key: prefix + base_name (e.g., "food_apple_red")
    std::string texture_key = prefix + base_name;

    // Create full path to the file
    std::string full_path = folder_path + "/" + file_info.name;

    // Load texture
    if (add_texture(texture_key, full_path)) {
      std::cout << "Loaded texture: " << texture_key << " from " << full_path << "\n";
      loaded_count++;
    } else {
      std::cerr << "Failed to load texture: " << texture_key << " from " << full_path << "\n";
    }
  }

  // Must have at least one valid texture loaded for this prefix
  if (loaded_count == 0) {
    std::cerr << "No valid textures loaded from " << folder_path << "\n";
    return false;
  }

  return true;
}

auto TextureCache::get_texture_keys(const std::string& prefix) const -> std::vector<std::string> {
  std::vector<std::string> keys;

  for (const auto& key : _textures | std::views::keys) {
    if (key.starts_with(prefix)) {
      keys.push_back(key);
    }
  }

  return keys;
}

TextureCache::~TextureCache() {
  for (auto& pair : _textures) {
    std::cerr << "Unloading Texture\n";
    /* TODO: FIX THIS */
    // UnloadTexture(pair.second);
  }
  _textures.clear();
}