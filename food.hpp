#pragma once
#include <raylib.h>

class Ant;

class Food {
 public:
  Food();
  Food(const Vector2& position);
  auto operator=(const Food& other) -> Food&;

  auto draw() const -> void;
  auto eat(Ant& ant) -> void;

  [[nodiscard]] auto get_position() const -> const Vector2&;
  [[nodiscard]] auto get_size() const -> float;
  [[nodiscard]] auto get_value() const -> float;
  [[nodiscard]] auto is_eaten() const -> bool;
  [[nodiscard]] auto get_bounds() const -> const Rectangle&;

 protected:
  float _value;
  Vector2 _position;
  bool _eaten;
  const float _size = 10.0f;
  Rectangle _bounds;

  auto update_bounds() -> void;
};
