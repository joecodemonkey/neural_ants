#include "ant.hpp"

#include <raylib.h>
#include <raylibdrawex.h>
#include <raylibmathex.h>
#include <raymath.h>

#include <brain.hpp>
#include <genome.hpp>
#include <population.hpp>
#include <resources.hpp>
#include <texture_cache.hpp>
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

// Drawing methods
auto Ant::draw(TextureCache& texture_cache) const -> void {
  if (_dead) {
    return;
  }

  draw_body(texture_cache);
  draw_direction();
  draw_bounding();
  draw_energy();
  draw_coordinates();
}

auto Ant::draw_body(TextureCache& texture_cache) const -> void {
  const Texture2D& texture = texture_cache.get_texture(_textureIndex);
  const auto& position = get_position();

  // TODO: Cleanup magic numbers
  const Rectangle source = {0, 0, 16.0F, 16.0F};
  const Rectangle dest = {position.x, position.y, 16.0F, 16.0F};
  const Vector2 origin = {8.0F, 8.0F};  // Center of 16x16 texture

  DrawTexturePro(texture, source, dest, origin, get_rotation(), WHITE);
}

auto Ant::draw_energy() const -> void {
  const auto textRect = get_coordinates_rect();
  const int lineX = static_cast<int>(std::round(textRect.x));

  float energyPercentage = _energy / STARTING_ENERGY;
  if (energyPercentage > 1.0F) {
    energyPercentage = 1.0F;
  }

  int lineLength = static_cast<int>(std::round(textRect.width * energyPercentage));
  const int lineHeight = static_cast<int>(std::round(textRect.height));
  const int lineY = static_cast<int>((textRect.y));

  auto lineColor = GREEN;
  if (energyPercentage < 0.25F) {
    lineColor = RED;
  } else if (energyPercentage < 0.50F) {
    lineColor = ORANGE;
  } else if (energyPercentage < 0.75F) {
    lineColor = YELLOW;
  }

  DrawRectangle(lineX, lineY, lineLength, lineHeight, lineColor);
}

auto Ant::draw_coordinates() const -> void {
  const auto& position = get_position();
  const std::string text = fmt::format("({:.2f}, {:.2f})", position.x, position.y);
  const auto coordinatesRect = get_coordinates_rect();

  DrawText(text.c_str(), coordinatesRect.x, coordinatesRect.y, FONT_SIZE, BLACK);
}

auto Ant::draw_direction() const -> void {
  const auto rotation = get_rotation();
  const auto& bounds = get_bounds();
  const auto& position = get_position();

  const float lineLength = static_cast<float>(bounds.width) * 2.0F;
  const float endX = position.x + cosf(rotation * DEG2RAD) * lineLength;
  const float endY = position.y + sinf(rotation * DEG2RAD) * lineLength;

  DrawLineEx(position, {endX, endY}, LINE_THICKNESS, RED);
}

auto Ant::draw_bounding() const -> void {
  const auto rotation = get_rotation();
  const auto& bounds = get_bounds();
  const auto& position = get_position();
  const auto color = BLUE;

  DrawCircleLinesV(position, get_radius(), color);
  DrawRectangleLinesExRot(bounds, position, rotation, 1.0F, color);
  DrawPixelV(position, color);
}

auto Ant::get_coordinates_rect() const -> Rectangle {
  const auto& position = get_position();
  const std::string text = fmt::format("({:.2f}, {:.2f})", position.x, position.y);

  Vector2 textSize = MeasureTextEx(GetFontDefault(), text.c_str(), FONT_SIZE, FONT_SPACING);
  int textX = static_cast<int>((position.x) + 1.5F - (textSize.x / 2.0F));
  int textY = static_cast<int>(((position.y) - (textSize.y / 2.0f + 20)));

  return {static_cast<float>(textX), static_cast<float>(textY), textSize.x, textSize.y};
}