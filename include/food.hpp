#pragma once
#include <raylib.h>

#include <nlohmann/json.hpp>

#include "surroundings.hpp"

class Ant;

class Food {
 public:
  static constexpr float DEFAULT_SIZE = 10.0F;

  Food();
  Food(const Vector2& position);
  Food(const nlohmann::json& j);
  auto operator=(const Food& other) -> Food&;
  auto operator==(const Food& other) const -> bool;

  auto draw() const -> void;
  auto eat(Ant& ant) -> void;

  [[nodiscard]] auto get_position() const -> const Vector2&;
  [[nodiscard]] auto get_size() const -> float;
  [[nodiscard]] auto get_value() const -> float;
  [[nodiscard]] auto is_eaten() const -> bool;
  auto reset(const Vector2& position) -> void;
  [[nodiscard]] auto get_bounds() const -> const Rectangle&;
  [[nodiscard]] auto get_radius() const -> float;

  auto to_json() const -> nlohmann::json;

 protected:
  float _value;
  Vector2 _position;
  bool _eaten;
  const float _size = DEFAULT_SIZE;
  Rectangle _bounds;

  auto update_bounds() -> void;
};
