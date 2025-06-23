#include <ui/behaviors/textured.hpp>

UI::Behaviors::Textured::Textured() : _defaultID(0) {};

auto UI::Behaviors::Textured::add_texture(const std::string& name, TextureID id) -> void {
  _textures[name] = id;
}

auto UI::Behaviors::Textured::get_texture(const std::string& name) const -> TextureID {
  auto it = _textures.find(name);
  return (it != _textures.end()) ? it->second : _defaultID;
}

auto UI::Behaviors::Textured::has_texture(const std::string& name) const -> bool {
  return _textures.find(name) != _textures.end();
}

auto UI::Behaviors::Textured::get_default() const -> TextureID {
  return _defaultID;
}

auto UI::Behaviors::Textured::set_default(TextureID id) -> void {
  _defaultID = id;
}