#pragma once

#include <string>
#include <unordered_map>

namespace UI::Behaviors {

class Textured {
 public:
  Textured();
  using TextureID = unsigned int;
  auto add_texture(const std::string& name, TextureID id) -> void;
  auto get_texture(const std::string& name) const -> TextureID;
  auto has_texture(const std::string& name) const -> bool;
  auto get_default() const -> TextureID;
  auto set_default(TextureID id) -> void;

 private:
  std::unordered_map<std::string, TextureID> _textures;
  TextureID _defaultID;
};
}  // namespace UI::Behaviors
