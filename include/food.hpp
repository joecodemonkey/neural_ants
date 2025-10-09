#pragma once
#include <raylib.h>

#include <nlohmann/json.hpp>
#include <cstddef>
#include <functional>

#include "surroundings.hpp"

class Ant;
class TextureCache;

class Food {
 public:
  static constexpr float TEXTURE_WIDTH = 16.0F;
  static constexpr float TEXTURE_HEIGHT = 16.0F;
  static constexpr float RADIUS = TEXTURE_HEIGHT / 2.0F;

  Food(TextureCache& textureCache);
  Food(const Vector2& position, TextureCache& textureCache);
  Food(const nlohmann::json& j, TextureCache& textureCache);
  auto operator=(const Food& other) -> Food&;
  auto operator==(const Food& other) const -> bool;

  auto draw() const -> void;
  auto eat(Ant& ant) -> void;

  [[nodiscard]] auto get_position() const -> const Vector2&;
  [[nodiscard]] auto get_value() const -> float;
  [[nodiscard]] auto is_eaten() const -> bool;
  auto reset(const Vector2& position) -> void;
  [[nodiscard]] auto get_bounds() const -> const Rectangle&;
  [[nodiscard]] auto get_radius() const -> float;

  [[nodiscard]] auto get_texture_index() const -> size_t;
  auto set_texture_index(size_t index) -> void;

  auto to_json() const -> nlohmann::json;

 protected:
  float _value;
  Vector2 _position;
  bool _eaten;
  Rectangle _bounds;
  size_t _textureIndex = 0;  // global texture index for rendering
  std::reference_wrapper<TextureCache> _textureCache;

  auto update_bounds() -> void;
};
