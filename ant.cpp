#include "ant.hpp"

#include <signal.h>

#include <cmath>
#include <iostream>

#include "population.hpp"
#include "raylib.h"
#include "raylibdrawex.h"
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
  _antTexturePath = other._antTexturePath;
  _antTexture = other._antTexture;
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

  draw_energy(draw_coordinates());
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
  update_position(time);
}

auto Ant::update_position(float time) -> void {
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

  // Consume stationary energy
  _energy -= SEDINTARY_ENERGY_PER_SECOND * time;

  // If the ant's energy is less than or equal to 0, set the ant to dead
  if (_energy <= 0.0F) {
    _energy = 0.0F;
    _dead = true;
  }
  if (_energy > STARTING_ENERGY) {
    _energy = STARTING_ENERGY;
  }
}

auto Ant::draw_body() -> void {
  if (!IsTextureValid(_antTexture)) {
    throw std::runtime_error("Ant texture is invalid - cannot draw ant body");
  }

  DrawTextureEx(_antTexture, {_bounds.x, _bounds.y}, get_rotation(), 1.0F, WHITE);
}

auto Ant::draw_energy(Rectangle const& text_rect) const -> void {
  const int lineX = static_cast<int>(std::round(text_rect.x));

  const float energyPercentage = _energy / STARTING_ENERGY;
  int lineLength = static_cast<int>(std::round(text_rect.width / 2.0F * energyPercentage));
  const int lineWidth = static_cast<int>(std::round(text_rect.height / 4.0F));
  const int lineY = static_cast<int>(std::round(text_rect.y + text_rect.height / 2.0F));

  auto lineColor = GREEN;
  if (_energy < 25) {
    lineColor = RED;
  } else if (_energy < 50) {
    lineColor = ORANGE;
  } else if (_energy < 75) {
    lineColor = YELLOW;
  }

  DrawRectangle(lineX, lineY, lineLength, lineWidth, lineColor);
}

auto Ant::draw_coordinates() const -> Rectangle {
  const std::string text = fmt::format("({:.2f}, {:.2f})", _position.x, _position.y);
  const auto FONT_SIZE = 20;
  const auto FONT_SPACING = 1.0F;

  // TODO: CLEAN THIS UP
  Vector2 textSize = MeasureTextEx(GetFontDefault(), text.c_str(), FONT_SIZE, FONT_SPACING);
  int textX =
      static_cast<int>(std::roundf(static_cast<float>(_position.x) + 1.5F - (textSize.x / 2.0F)));
  int textY =
      static_cast<int>(std::roundf(static_cast<float>(_position.y) - (textSize.y / 2.0f + 20)));

  DrawText(text.c_str(), textX, textY, FONT_SIZE, BLACK);

  return {static_cast<float>(textX), static_cast<float>(textY), textSize.x, textSize.y};
}

[[nodiscard]] auto Ant::get_texture_path() const -> const std::string& {
  return _antTexturePath;
}

auto Ant::set_texture_path(std::string const& path) -> void {
  _antTexture = LoadTexture(path.c_str());

  if (!IsTextureValid(_antTexture)) {
    throw std::runtime_error("Failed to load ant texture at path: " + path);
  }

  _antTexturePath = path;
  update_bounds();
  update_radius();
}

auto Ant::draw_direction() const -> void {
  const float LINE_THICKNESS = 2.0F;
  const auto rotation = get_rotation();

  // Calculate line length as 2x the texture width
  const float lineLength = static_cast<float>(_antTexture.width) * 2.0F;

  // Calculate end point using rotation
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
  // Draw a line showing the direction the ant is facing
  const auto rotation = get_rotation();
  const float width = static_cast<float>(_antTexture.width);
  const float height = static_cast<float>(_antTexture.height);

  // Calculate the offset from center
  const double offsetX = _antTexture.width / 2.0F;
  const double offsetY = _antTexture.height / 2.0F;

  // Rotate the offset vector
  const double cos_rot = cos(rotation * DEG2RAD);
  const double sin_rot = sin(rotation * DEG2RAD);
  double pos_x_adj = offsetX * cos_rot - offsetY * sin_rot;
  double pos_y_adj = offsetX * sin_rot + offsetY * cos_rot;

  Vector2 adj_draw_position = Vector2Zero();
  adj_draw_position.x = _position.x - pos_x_adj;
  adj_draw_position.y = _position.y - pos_y_adj;

  _bounds.x = adj_draw_position.x;
  _bounds.y = adj_draw_position.y;
  _bounds.width = _antTexture.width;
  _bounds.height = _antTexture.height;
}

[[nodiscard]] auto Ant::get_rotation() const -> float {
  return atan2f(_velocity.y, _velocity.x) * RAD2DEG;
}

auto Ant::update_radius() -> void {
  const auto width_sq = _antTexture.width * _antTexture.width;
  const auto height_sq = _antTexture.height * _antTexture.height;
  _radius = sqrtf((width_sq + height_sq)) / 2.0F;
}

auto Ant::collides(const Vector2& position, float radius) const -> bool {
  // Calculate the squared distance between the centers of the two circles
  float dx = _position.x - position.x;
  float dy = _position.y - position.y;
  float distanceSquared = dx * dx + dy * dy;

  float sumOfRadii = _radius + radius;
  return distanceSquared < (sumOfRadii * sumOfRadii);
}