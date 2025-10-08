#pragma once

#include <raylib.h>

#include <nlohmann/json.hpp>
#include <population.hpp>
#include <resources.hpp>
#include <texture_cache.hpp>
#include <surroundings.hpp>

class World {
 public:
  World();
  World(const nlohmann::json& j);

  World(const World& other);

  World(World&& other) noexcept;

  World& operator=(const World& other);

  World& operator=(World&& other) noexcept;

  bool operator==(const World& other) const;

  [[nodiscard]] auto get_population() const -> const Population&;
  [[nodiscard]] auto get_population() -> Population&;

  [[nodiscard]] auto get_resources() -> Resources&;
  [[nodiscard]] auto get_resources() const -> const Resources&;

  [[nodiscard]] auto get_bounds() const -> const Rectangle&;

  [[nodiscard]] auto get_spawn_bounds() const -> const Rectangle&;
  auto set_spawn_margin(float) -> void;
  [[nodiscard]] auto get_spawn_margin() const -> float;

  auto update(float time) -> void;
  auto draw() -> void;

  [[nodiscard]] auto out_of_bounds(const Vector2& position) const -> bool;

  [[nodiscard]] auto spawn_position(const Vector2& dimensions) const -> Vector2;

  auto to_json() const -> nlohmann::json;

  auto set_texture_cache(std::shared_ptr<TextureCache> cache) -> void;
  auto get_texture_cache() -> std::shared_ptr<TextureCache>&;

 protected:
  const Rectangle DEFAULT_BOUNDS = {0.0f, 0.0f, 1000.0f, 1000.0f};
  const float DEFAULT_SPAWN_MARGIN = 0.20F;

  auto update_spawn_rect() -> void;
  Resources _resources;
  Population _population;
  Rectangle _bounds;
  Rectangle _spawnBounds;
  std::shared_ptr<TextureCache> _textureCache;
  float _spawnMargin;
};
