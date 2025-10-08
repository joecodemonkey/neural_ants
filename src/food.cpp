#include "food.hpp"

#include <genome.hpp>

#include "ant.hpp"
#include "raylib.h"
#include "texture_cache.hpp"
#include "util/serialization.hpp"

Food::Food(TextureCache* textureCache)
    : _value(500.0f), _position(0.0f, 0.0f), _eaten(false), _textureCache(textureCache) {
  if (_textureCache) {
    _textureIndex = _textureCache->get_random_texture_index("food_");
  }
  update_bounds();
}

Food::Food(const Vector2& position, TextureCache* textureCache)
    : _value(500.0f), _position(position), _eaten(false), _textureCache(textureCache) {
  if (_textureCache) {
    _textureIndex = _textureCache->get_random_texture_index("food_");
  }
  update_bounds();
}

auto Food::operator=(const Food& other) -> Food& {
  if (this != &other) {
    // Check for self-assignment
    _position = other._position;
    _value = other._value;
    _eaten = other._eaten;
    _bounds = other._bounds;
    _textureIndex = other._textureIndex;
    _textureCache = other._textureCache;
  }
  return *this;
}

Food::Food(const nlohmann::json& j, TextureCache* textureCache) : _textureCache(textureCache) {
  _value = j.at("value").get<float>();
  _position = Util::vector2_from_json(j.at("position"));
  _eaten = j.at("eaten").get<bool>();

  // Load texture index from JSON, or randomize if not present
  if (j.contains("textureIndex")) {
    _textureIndex = j.at("textureIndex").get<size_t>();
  } else if (_textureCache) {
    _textureIndex = _textureCache->get_random_texture_index("food_");
  }

  update_bounds();
}

auto Food::operator==(const Food& other) const -> bool {
  return _position.x == other._position.x && _position.y == other._position.y &&
         _value == other._value && _eaten == other._eaten;
}

auto Food::draw() const -> void {
  if (_eaten || !_textureCache)
    return;

  const Texture2D& texture = _textureCache->get_texture(_textureIndex);
  const Rectangle source = {0, 0, TEXTURE_WIDTH, TEXTURE_HEIGHT};
  const Rectangle dest = {_position.x, _position.y, TEXTURE_WIDTH, TEXTURE_HEIGHT};
  const Vector2 origin = {TEXTURE_WIDTH / 2.0F, TEXTURE_HEIGHT / 2.0F};  // Center of 16x16 texture

  DrawTexturePro(texture, source, dest, origin, 0.0F, WHITE);
}

auto Food::eat(Ant& ant) -> void {
  if (_eaten)
    return;
  _eaten = true;
  ant.set_energy(ant.get_energy() + _value);
}

auto Food::get_bounds() const -> const Rectangle& {
  return _bounds;
}

auto Food::get_position() const -> const Vector2& {
  return _position;
}


auto Food::get_value() const -> float {
  return _value;
}

auto Food::is_eaten() const -> bool {
  return _eaten;
}

auto Food::update_bounds() -> void {
  _bounds.x = _position.x - (TEXTURE_WIDTH / 2.0F);
  _bounds.y = _position.y - (TEXTURE_HEIGHT / 2.0F);
  _bounds.width = TEXTURE_WIDTH;
  _bounds.height = TEXTURE_HEIGHT;
}

auto Food::reset(const Vector2& position) -> void {
  _eaten = false;
  _position = position;
}

[[nodiscard]] auto Food::get_radius() const -> float {
  return RADIUS;
}

auto Food::to_json() const -> nlohmann::json {
  nlohmann::json j;
  j["value"] = _value;
  j["position"] = Util::vector2_to_json(_position);
  j["eaten"] = _eaten;
  j["textureIndex"] = _textureIndex;
  return j;
}

auto Food::get_texture_index() const -> size_t {
  return _textureIndex;
}

auto Food::set_texture_index(size_t index) -> void {
  _textureIndex = index;
}
