#pragma once
#include <fmt/core.h>

#include <string>

#include "raylib.h"

class Ant {
 public:
  Ant() {};

  Ant& operator=(const Ant& other) {
    if (this != &other) {
      // Check for self-assignment
      _position = other._position;
      _speed = other._speed;
      _direction = other._direction;
      _dead = other._dead;
      _energy = other._energy;
      _lifeSpan = other._lifeSpan;
    }
    return *this;
  }

  Ant(const Ant& other) {
    _position = other._position;
    _speed = other._speed;
    _direction = other._direction;
    _dead = other._dead;
    _energy = other._energy;
    _lifeSpan = other._lifeSpan;
    _antTexturePath = other._antTexturePath;
    _antTexture = other._antTexture;
  }
  ~Ant() = default;

  void update();

  void draw();

  [[nodiscard]] float get_direction() const {
    return _direction;
  }
  void set_direction(float direction) {
    _direction = direction;
  }

  [[nodiscard]] float get_energy() const {
    return _energy;
  }
  void set_energy(float energy) {
    _energy = energy;
  }

  [[nodiscard]] float get_speed() const {
    return _speed;
  }
  void set_speed(float speed) {
    _speed = speed;
  }

  [[nodiscard]] bool is_dead() const {
    return _dead;
  }
  void set_dead(bool dead) {
    _dead = dead;
  }

  [[nodiscard]] float get_life_span() const {
    return _lifeSpan;
  }

  [[nodiscard]] const Vector2& get_position() const {
    return _position;
  }
  void set_position(const Vector2& position) {
    _position = position;
  }

  auto reset(const Vector2& position) -> void;

  [[nodiscard]] float const get_size() const {
    return SIZE;
  }

  void set_ant_texture_path(std::string const& path);
  [[nodiscard]] std::string const& get_ant_texture_path() const {
    return _antTexturePath;
  }

  [[nodiscard]] Vector2 get_size();

 protected:
  const float SIZE = 20.0f;

  Vector2 _position = {0.0f, 0.0f};
  float _speed = 0.0f;
  float _direction = 0.0f;
  bool _dead = false;
  bool _frozen = false;

  // All Ants start with the same amount of energy
  const float STARTING_ENERGY = 100.0f;
  float _energy = STARTING_ENERGY;

  // Lifespan records the number of seconds the ant has been alive
  float _lifeSpan = 0.0f;

  // sedintary energy per second is the base rate of energy loss for a stationary ant
  const float SEDINTARY_ENERGY_PER_SECOND = 0.1f;

  // Update methods
  void update_energy();
  void update_position();

  void draw_body();
  void draw_energy(Rectangle const& text_rect);
  Rectangle draw_coordinates();
  void draw_direction();

  Texture2D _antTexture;
  std::string _antTexturePath;
};
