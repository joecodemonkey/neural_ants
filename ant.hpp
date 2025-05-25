#pragma once
#include <fmt/core.h>
#include <raylib.h>

#include "raymath.h"

class World;

class Ant {
 public:
  Ant(World& world);

  auto operator=(const Ant& other) -> Ant&;

  Ant(const Ant& other);

  ~Ant() = default;

  auto draw() -> void;

  auto update(float time) -> void;

  [[nodiscard]] auto get_direction() const -> float;

  [[nodiscard]] auto get_energy() const -> float;
  auto set_energy(float energy) -> void;

  [[nodiscard]] auto get_speed() const -> float;
  auto set_speed(float speed) -> void;

  [[nodiscard]] auto is_dead() const -> bool;
  auto set_dead(bool dead) -> void;

  [[nodiscard]] auto get_life_span() const -> float;

  [[nodiscard]] auto get_position() const -> const Vector2&;
  auto set_position(const Vector2& position) -> void;

  [[nodiscard]] auto get_scale() const -> float;
  auto set_scale(float scale) -> void;

  [[nodiscard]] auto get_texture_path() const -> const std::string&;
  auto set_texture_path(std::string const& path) -> void;

  [[nodiscard]] auto get_bounds() const -> const Rectangle&;

  auto reset(const Vector2& position) -> void;
  auto collides(const Vector2& position, float radius) const -> bool;

 protected:
  const float DEFAULT_SCALE = 20.0F;
  const float STARTING_ENERGY = 1000.0F;
  // sedintary energy per second is the base rate of energy loss for a stationary ant
  const float SEDINTARY_ENERGY_PER_SECOND = 1.0F;
  const float MAX_VELOCITY = 30.0f;
  const float LINE_THICKNESS = 2.0F;

  World& _world;

  Vector2 _position = Vector2Zero();
  Vector2 _velocity = Vector2Zero();
  Rectangle _bounds = {0.0F, 0.0F, 0.0F, 0.0F};
  float _radius = 0.0F;
  bool _dead = false;
  bool _frozen = false;
  float _scale = DEFAULT_SCALE;
  Texture2D _texture;
  std::string _texturePath;
  // TODO: Restore this
  // Brain _brain;
  float _energy = STARTING_ENERGY;
  float _lifeSpan = 0.0F;  // time in seconds ant has been alive (measure of fitness)

  // Update methods
  auto update_energy(float time) -> void;
  auto update_bounds() -> void;
  auto update_radius() -> void;

  auto draw_body() -> void;
  auto draw_energy(Rectangle const& text_rect) const -> void;
  [[nodiscard]] auto draw_coordinates() const -> Rectangle;
  auto draw_direction() const -> void;
  auto draw_bounding() const -> void;

  [[nodiscard]] auto get_rotation() const -> float;
};
