#include "ant.hpp"

#include "population.hpp"
#include "raylib.h"
#include "raylibdrawex.h"
#include "raylibmathex.h"
#include "raymath.h"
#include "resources.hpp"
#include "world.hpp"

Ant::Ant(World& world) : _world(world) {}

auto Ant::operator=(const Ant& other) -> Ant& {
  if (this != &other) {
    // Check for self-assignment
    _position = other._position;
    _velocity = other._velocity;
    _dead = other._dead;
    _energy = other._energy;
    _lifeSpan = other._lifeSpan;
    _texturePath = other._texturePath;
    _texture = other._texture;
    _radius = other._radius;
    _bounds = other._bounds;
  }
  return *this;
}

Ant::Ant(const Ant& other) : _world(other._world) {
  _position = other._position;
  _velocity = other._velocity;
  _dead = other._dead;
  _energy = other._energy;
  _lifeSpan = other._lifeSpan;
  _texturePath = other._texturePath;
  _texture = other._texture;
  _radius = other._radius;
  _bounds = other._bounds;
}

auto Ant::draw() -> void {
  if (_dead) {
    return;  // he's not dead, he's just resting
  }

  draw_body();

  draw_direction();
  draw_bounding();
  draw_energy();
  draw_coordinates();
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

auto Ant::get_position() const -> const Vector2& {
  return _position;
}

auto Ant::set_position(const Vector2& position) -> void {
  _position = position;
}

[[nodiscard]] auto Ant::get_scale() const -> float {
  return _scale;
}

auto Ant::set_scale(float scale) -> void {
  _scale = scale;
}

auto Ant::update(float time) -> void {
  update_energy(time);
  if (_dead) {
    return;  // he's not dead, he's just resting
  }

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

auto Ant::draw_body() -> void {
  if (!IsTextureValid(_texture)) {
    throw std::runtime_error("Ant texture is invalid - cannot draw ant body");
  }

  DrawTextureEx(_texture, {_bounds.x, _bounds.y}, get_rotation(), 1.0F, WHITE);
}

auto Ant::draw_energy() const -> void {
  const auto text_rect = get_coordinates_rect();
  const int lineX = static_cast<int>(std::round(text_rect.x));

  float energyPercentage = _energy / STARTING_ENERGY;
  if (energyPercentage > 1.0F) {
    energyPercentage = 1.0F;  // keep the bar from getting too long
  }

  int lineLength = static_cast<int>(std::round(text_rect.width * energyPercentage));
  const int lineHeight = static_cast<int>(std::round(text_rect.height));
  const int lineY = static_cast<int>((text_rect.y));

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

auto Ant::get_coordinates_rect() const -> Rectangle {
  const std::string text = fmt::format("({:.2f}, {:.2f})", _position.x, _position.y);

  // TODO: CLEAN THIS UP
  Vector2 textSize = MeasureTextEx(GetFontDefault(), text.c_str(), FONT_SIZE, FONT_SPACING);
  int textX = static_cast<int>((_position.x) + 1.5F - (textSize.x / 2.0F));
  int textY = static_cast<int>(((_position.y) - (textSize.y / 2.0f + 20)));

  return {static_cast<float>(textX), static_cast<float>(textY), textSize.x, textSize.y};
}

auto Ant::draw_coordinates() const -> void {
  const std::string text = fmt::format("({:.2f}, {:.2f})", _position.x, _position.y);
  const auto coordinates_rect = get_coordinates_rect();

  DrawText(text.c_str(), coordinates_rect.x, coordinates_rect.y, FONT_SIZE, BLACK);
}

[[nodiscard]] auto Ant::get_texture_path() const -> const std::string& {
  return _texturePath;
}

auto Ant::set_texture_path(std::string const& path) -> void {
  _texture = LoadTexture(path.c_str());

  if (!IsTextureValid(_texture)) {
    throw std::runtime_error("Failed to load ant texture at path: " + path);
  }

  _texturePath = path;
  update_bounds();
  update_radius();
}

auto Ant::draw_direction() const -> void {
  const auto rotation = get_rotation();

  const float lineLength = static_cast<float>(_texture.width) * 2.0F;
  const float endX = _position.x + cosf(rotation * DEG2RAD) * lineLength;
  const float endY = _position.y + sinf(rotation * DEG2RAD) * lineLength;

  DrawLineEx(_position, {endX, endY}, LINE_THICKNESS, RED);
}

auto Ant::draw_bounding() const -> void {
  const auto rotation = get_rotation();
  const auto color = BLUE;

  DrawCircleLinesV(_position, _radius, color);
  DrawRectangleLinesExRot(_bounds, _position, rotation, 1.0F, color);
  DrawPixelV(_position, color);
}

auto Ant::reset(const Vector2& position) -> void {
  // Generate random x and y components
  float x = GetRandomValue(-100, 100);
  float y = GetRandomValue(-100, 100);
  // Normalize and scale to random magnitude up to MAX_VELOCITY
  float magnitude = GetRandomValue(0, static_cast<int>(MAX_VELOCITY));
  _velocity = Vector2Scale(Vector2Normalize({x, y}), magnitude);
  _position = position;
  _energy = STARTING_ENERGY;
  _dead = false;
}

auto Ant::get_bounds() const -> const Rectangle& {
  return _bounds;
}

auto Ant::update_bounds() -> void {
  _bounds.width = _texture.width;
  _bounds.height = _texture.height;
  _bounds = RotateRect(_bounds, _position, get_rotation());
}

[[nodiscard]] auto Ant::get_rotation() const -> float {
  return atan2f(_velocity.y, _velocity.x) * RAD2DEG;
}

auto Ant::update_radius() -> void {
  const auto width_sq = _texture.width * _texture.width;
  const auto height_sq = _texture.height * _texture.height;
  _radius = sqrtf((width_sq + height_sq)) / 2.0F;
}

auto Ant::collides(const Vector2& position, float radius) const -> bool {
  return CheckCollisionCircles(position, radius, _position, _radius);
}