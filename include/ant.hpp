#pragma once
#include <fmt/core.h>
#include <raylib.h>
#include <raymath.h>

#include <brain.hpp>
#include <genome.hpp>
#include <nlohmann/json.hpp>

class World;

class Ant {
 public:
  static constexpr float MAX_VELOCITY = 30.0f;

  Ant(World& world, const Genome& genome);
  Ant(const nlohmann::json& json, World& world);

  auto operator=(const Ant& other) -> Ant&;

  auto operator==(const Ant& other) const -> bool;

  Ant(const Ant& other);

  ~Ant();

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
  auto set_life_span(float life_span) -> void;

  [[nodiscard]] auto get_position() const -> const Vector2&;
  auto set_position(const Vector2& position) -> void;

  [[nodiscard]] auto get_scale() const -> float;
  auto set_scale(float scale) -> void;

  [[nodiscard]] auto get_bounds() const -> const Rectangle&;

  auto reset(const Vector2& position) -> void;
  auto collides(const Vector2& position, float radius) const -> bool;

  auto set_velocity(const Vector2 velocity) -> void;
  auto get_velocity() const -> const Vector2&;

  auto get_genome() const -> Genome;

  auto to_json() const -> nlohmann::json;

 protected:
  auto set_texture(Texture2D texture) -> void;

  auto create_ant() -> Ant;

  const float DEFAULT_SCALE = 20.0F;
  const float STARTING_ENERGY = 1000.0F;
  const float SEDINTARY_ENERGY_PER_SECOND = 1.0F;
  const float LINE_THICKNESS = 2.0F;
  const float FONT_SIZE = 10.0F;
  const float FONT_SPACING = 1.0F;

  World& _world;
  Genome _genome;
  Brain _brain;

  Vector2 _position = Vector2Zero();
  Vector2 _velocity = Vector2Zero();
  Rectangle _bounds = {0.0F, 0.0F, 0.0F, 0.0F};
  float _radius = 0.0F;
  bool _dead = false;
  bool _frozen = false;
  float _scale = DEFAULT_SCALE;
  Texture2D _texture;
  float _energy = STARTING_ENERGY;
  float _lifeSpan = 0.0F;  // time in seconds ant has been alive (measure of fitness)

  // Update methods
  auto update_energy(float time) -> void;
  auto update_bounds() -> void;
  auto update_radius() -> void;

  auto draw_body() -> void;
  auto draw_energy() const -> void;
  auto draw_coordinates() const -> void;
  [[nodiscard]] auto get_coordinates_rect() const -> Rectangle;
  auto draw_direction() const -> void;
  auto draw_bounding() const -> void;

  [[nodiscard]] auto get_rotation() const -> float;
};
