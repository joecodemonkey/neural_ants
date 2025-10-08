#pragma once
#include <fmt/core.h>
#include <raylib.h>
#include <raymath.h>

#include <functional>
#include <brain.hpp>
#include <genome.hpp>
#include <nlohmann/json.hpp>

class World;

class Ant {
 public:
  static constexpr float MAX_VELOCITY = 30.0f;
  static constexpr int ANT_LIVES = 10;

  Ant(World& world, const Genome& genome);
  Ant(const nlohmann::json& json, World& world);

  auto operator=(const Ant& other) -> Ant&;

  auto operator==(const Ant& other) const -> bool;

  Ant(const Ant& other);

  ~Ant();

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

  [[nodiscard]] auto get_remaining_lives() const -> int;
  auto set_remaining_lives(int remaining_lives) -> void;

  [[nodiscard]] auto get_cumulative_life_span() const -> double;
  auto set_cumulative_life_span(double cumulative_life_span) -> void;

  [[nodiscard]] auto get_position() const -> const Vector2&;
  auto set_position(const Vector2& position) -> void;

  [[nodiscard]] auto get_radius() const -> float { return RADIUS; };

  [[nodiscard]] auto get_bounds() const -> const Rectangle& { return _bounds; }

  auto reset(const Vector2& position) -> void;
  auto collides(const Vector2& position, float radius) const -> bool;

  auto set_velocity(const Vector2 velocity) -> void;
  auto get_velocity() const -> const Vector2&;

  auto get_genome() const -> Genome;

  auto to_json() const -> nlohmann::json;

  [[nodiscard]] auto get_texture_index() const -> size_t;
  auto set_texture_index(size_t index) -> void;

 protected:
  auto create_ant() -> Ant;

  const float STARTING_ENERGY = 1000.0F;
  const float SEDINTARY_ENERGY_PER_SECOND = 1.0F;
  static constexpr float TEXTURE_WIDTH = 16.0F;
  static constexpr float TEXTURE_HEIGHT = TEXTURE_WIDTH;
  static const Rectangle BOUNDS;
  static constexpr float RADIUS = TEXTURE_HEIGHT / 2.0;  // Half of 16x16 texture (proper circle radius)
  std::reference_wrapper<World> _world;
  Genome _genome;
  Brain _brain;

  Rectangle _bounds = BOUNDS;

  Vector2 _position = Vector2Zero();
  Vector2 _velocity = Vector2Zero();
  bool _dead = false;
  bool _frozen = false;
  float _energy = STARTING_ENERGY;
  float _lifeSpan = 0.0F;            // time in seconds ant has been alive (measure of fitness)
  int _remainingLives = ANT_LIVES;   // number of lives remaining
  double _cumulativeLifeSpan = 0.0;  // sum of all life spans from all lives
  size_t _textureIndex = 0;          // global texture index for rendering

  // Update methods
  auto update_energy(float time) -> void;
  auto update_bounds() -> void;

  [[nodiscard]] auto get_rotation() const -> float;
};
