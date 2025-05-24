#pragma once
#include <raylib.h>

#include "ant.hpp"

class Food {
 public:
  Food();
  Food(const Vector2& position);
  Food& operator=(const Food& other);

  void draw() const;
  void eat(Ant& ant);

  [[nodiscard]] auto get_position() const -> const Vector2&;
  [[nodiscard]] float get_size() const;
  [[nodiscard]] float get_value() const;
  [[nodiscard]] auto is_eaten() const -> bool;
  [[nodiscard]] auto get_bounds() const -> const Rectangle&;

 protected:
  float _value;
  Vector2 _position;
  bool _eaten;
  const float _size = 10.0f;
  Rectangle _bounds;

  void update_rect();
};
