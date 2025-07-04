#include "food.hpp"

#include <genome.hpp>

#include "ant.hpp"
#include "raylib.h"
#include "util/serialization.hpp"

Food::Food() : _value(10.0f), _position(0.0f, 0.0f), _eaten(false) {
  update_bounds();
}

Food::Food(const Vector2& position) : _value(10.0f), _position(position), _eaten(false) {
  update_bounds();
}

auto Food::operator=(const Food& other) -> Food& {
  if (this != &other) {
    // Check for self-assignment
    _position = other._position;
    _value = other._value;
    _eaten = other._eaten;
    _bounds = other._bounds;
  }
  return *this;
}

Food::Food(const nlohmann::json& j) {
  _value = j.at("value").get<float>();
  _position = Util::vector2_from_json(j.at("position"));
  _eaten = j.at("eaten").get<bool>();
  update_bounds();
}

auto Food::operator==(const Food& other) const -> bool {
  return _position.x == other._position.x && _position.y == other._position.y &&
         _value == other._value && _eaten == other._eaten;
}

auto Food::draw() const -> void {
  if (_eaten)
    return;
  DrawCircle(_position.x, _position.y, _size / 2, GREEN);
  DrawPixel(_position.x, _position.y, RED);
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

auto Food::get_size() const -> float {
  return _size;
}

auto Food::get_value() const -> float {
  return _value;
}

auto Food::is_eaten() const -> bool {
  return _eaten;
}

auto Food::update_bounds() -> void {
  _bounds.x = _position.x / 2;
  _bounds.y = _position.y / 2;
  _bounds.width = _size;
  _bounds.height = _size;
}

auto Food::reset(const Vector2& position) -> void {
  _eaten = false;
  _position = position;
}

[[nodiscard]] auto Food::get_radius() const -> float {
  return _size / 2;
}

auto Food::to_json() const -> nlohmann::json {
  nlohmann::json j;
  j["value"] = _value;
  j["position"] = Util::vector2_to_json(_position);
  j["eaten"] = _eaten;
  j["size"] = _size;
  return j;
}
