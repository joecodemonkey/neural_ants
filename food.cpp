#include "food.hpp"

#include "ant.hpp"
#include "raylib.h"

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

auto Food::draw() const -> void {
  if (_eaten)
    return;
  DrawCircle(_position.x, _position.y, _size / 2, GREEN);
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
  _bounds.x = _position.x;
  _bounds.y = _position.y;
  _bounds.width = _size;
  _bounds.height = _size;
}
