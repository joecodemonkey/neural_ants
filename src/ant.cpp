#include "ant.hpp"

#include <raylib.h>
#include <raylibmathex.h>
#include <raymath.h>

#include <brain.hpp>
#include <genome.hpp>
#include <population.hpp>
#include <resources.hpp>
#include <util/serialization.hpp>
#include <world.hpp>

const Rectangle Ant::BOUNDS = {0.0F, 0.0F, Ant::TEXTURE_WIDTH, Ant::TEXTURE_HEIGHT};

Ant::Ant(World& world, const Genome& genome)
    : _world(world), _brain(world, genome.get_network()), _genome(genome) {}

auto Ant::operator=(const Ant& other) -> Ant& {
  if (this != &other) {
    // Check for self-assignment
    _position = other._position;
    _velocity = other._velocity;
    _dead = other._dead;
    _energy = other._energy;
    _lifeSpan = other._lifeSpan;
    _remainingLives = other._remainingLives;
    _cumulativeLifeSpan = other._cumulativeLifeSpan;
    _bounds = other._bounds;
    _textureIndex = other._textureIndex;
    _genome = other._genome;
    _brain = other._brain;
  }
  return *this;
}

auto Ant::operator==(const Ant& other) const -> bool {
  return Vector2Equals(_position, other._position) && Vector2Equals(_velocity, other._velocity) &&
         _dead == other._dead && _energy == other._energy && _lifeSpan == other._lifeSpan &&
         _remainingLives == other._remainingLives &&
         _cumulativeLifeSpan == other._cumulativeLifeSpan &&
         _frozen == other._frozen && _genome == other._genome;
}

Ant::Ant(const Ant& other)
    : _world(other._world),
      _genome(other._genome),
      _brain(other._world, other._genome.get_network()) {
  _position = other._position;
  _velocity = other._velocity;
  _dead = other._dead;
  _energy = other._energy;
  _lifeSpan = other._lifeSpan;
  _remainingLives = other._remainingLives;
  _cumulativeLifeSpan = other._cumulativeLifeSpan;
  _textureIndex = other._textureIndex;
}

[[nodiscard]] auto Ant::is_dead() const -> bool {
  return _dead;
}

auto Ant::set_dead(bool dead) -> void {
  _dead = dead;
}

auto Ant::get_energy() const -> float {
  return _energy;
}

auto Ant::set_energy(float energy) -> void {
  _energy = energy;
}

auto Ant::get_life_span() const -> float {
  return _lifeSpan;
}

auto Ant::set_life_span(float life_span) -> void {
  _lifeSpan = life_span;
}

auto Ant::get_remaining_lives() const -> int {
  return _remainingLives;
}

auto Ant::set_remaining_lives(int remainingLives) -> void {
  _remainingLives = remainingLives;
}

auto Ant::get_cumulative_life_span() const -> double {
  return _cumulativeLifeSpan;
}

auto Ant::set_cumulative_life_span(double cumulativeLifeSpan) -> void {
  _cumulativeLifeSpan = cumulativeLifeSpan;
}

auto Ant::get_position() const -> const Vector2& {
  return _position;
}

auto Ant::set_position(const Vector2& position) -> void {
  _position = position;
}

auto Ant::update(float time) -> void {
  update_energy(time);
  if (_dead) {
    return;  // he's not dead, he's just resting
  }
  _velocity = _brain.update(time, _position);
  _lifeSpan += time;
  _position = Vector2Add(_position, Vector2Scale(_velocity, time));
  update_bounds();
}

auto Ant::update_energy(float time) -> void {
  if (time == 0.0F || _dead) {
    return;
  }

  if (!Vector2Equals(_velocity, Vector2Zero())) {
    _energy -= Vector2Length(Vector2Scale(_velocity, time));
  }

  _energy -= SEDINTARY_ENERGY_PER_SECOND * time;  // Consume stationary energy

  // If the ant's energy is less than or equal to 0, set the ant to dead
  if (_energy <= 0.0F) {
    _energy = 0.0F;
    _dead = true;
  }
}

auto Ant::reset(const Vector2& position) -> void {
  float x = GetRandomValue(-100, 100);
  float y = GetRandomValue(-100, 100);
  float magnitude = GetRandomValue(0, static_cast<int>(MAX_VELOCITY));
  _velocity = Vector2Scale(Vector2Normalize({x, y}), magnitude);
  _position = position;
  _energy = STARTING_ENERGY;
  _dead = false;
  _lifeSpan = 0.0F;
  update_bounds();
}

auto Ant::update_bounds() -> void {
  _bounds.width = TEXTURE_WIDTH;
  _bounds.height = TEXTURE_HEIGHT;
  _bounds = RotateRect(_bounds, _position, get_rotation());
}

[[nodiscard]] auto Ant::get_rotation() const -> float {
  return atan2f(_velocity.y, _velocity.x) * RAD2DEG;
}

auto Ant::collides(const Vector2& position, float radius) const -> bool {
  return CheckCollisionCircles(position, radius, _position, RADIUS);
}

auto Ant::set_velocity(const Vector2 velocity) -> void {
  _velocity = velocity;
}
auto Ant::get_velocity() const -> const Vector2& {
  return _velocity;
}

Ant::~Ant() {}

auto Ant::get_genome() const -> Genome {
  return _genome;
}

auto Ant::to_json() const -> nlohmann::json {
  nlohmann::json j;
  j["position"] = Util::vector2_to_json(_position);
  j["velocity"] = Util::vector2_to_json(_velocity);
  j["bounds"] = Util::rectangle_to_json(_bounds);
  j["dead"] = _dead;
  j["frozen"] = _frozen;
  j["energy"] = _energy;
  j["life_span"] = _lifeSpan;
  j["remaining_lives"] = _remainingLives;
  j["cumulative_life_span"] = _cumulativeLifeSpan;
  j["texture_index"] = _textureIndex;
  j["genome"] = _genome.to_json();
  return j;
}

Ant::Ant(const nlohmann::json& json, World& world)
    : _world(world), _genome(json.at("genome")), _brain(world, _genome.get_network()) {
  _position = Util::vector2_from_json(json.at("position"));
  _velocity = Util::vector2_from_json(json.at("velocity"));
  _bounds = Util::rectangle_from_json(json.at("bounds"));

  _dead = json.at("dead").get<bool>();
  _frozen = json.at("frozen").get<bool>();
  _energy = json.at("energy").get<float>();
  _lifeSpan = json.at("life_span").get<float>();
  _remainingLives = json.at("remaining_lives").get<int>();
  _cumulativeLifeSpan = json.at("cumulative_life_span").get<double>();
  _textureIndex = json.at("texture_index").get<size_t>();
}

auto Ant::get_texture_index() const -> size_t {
  return _textureIndex;
}

auto Ant::set_texture_index(size_t index) -> void {
  _textureIndex = index;
}