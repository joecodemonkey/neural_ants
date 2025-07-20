#include "ant_renderer.hpp"

#include <fmt/core.h>
#include <raylib.h>
#include <raylibdrawex.h>
#include <raylibmathex.h>
#include <raymath.h>

#include "ant.hpp"
#include "population.hpp"
#include "texture_cache.hpp"

AntRenderer::AntRenderer() = default;

AntRenderer::~AntRenderer() = default;

auto AntRenderer::set_texture_cache(TextureCache* textureCache) -> void {
  _textureCache = textureCache;
  _textureLoaded = false;
}

auto AntRenderer::set_scale(float scale) -> void {
  _scale = scale;
}

auto AntRenderer::draw(Population& population) -> void {
  if (!_textureLoaded) {
    if (load_ant_texture()) {
      population.set_texture_dimensions(_antTexture.width, _antTexture.height);
    }
  }

  auto& ants = population.get_ants();
  for (const auto& ant : ants) {
    draw_ant(ant);
  }
}

auto AntRenderer::load_ant_texture() -> bool {
  if (!_textureCache) {
    return false;
  }

  if (!_textureCache->has_texture("ant")) {
    return false;
  }

  _antTexture = _textureCache->get_texture("ant");
  _textureLoaded = true;
  return true;
}

auto AntRenderer::is_texture_valid() const -> bool {
  return _textureLoaded && IsTextureValid(_antTexture);
}

auto AntRenderer::draw_ant(const Ant& ant) -> void {
  if (ant.is_dead()) {
    return;
  }

  draw_body(ant);
  draw_direction(ant);
  draw_bounding(ant);
  draw_energy(ant);
  draw_coordinates(ant);
}

auto AntRenderer::draw_body(const Ant& ant) -> void {
  if (is_texture_valid()) {
    const auto& bounds = ant.get_bounds();
    DrawTextureEx(_antTexture, {bounds.x, bounds.y}, get_rotation(ant), 1.0F, WHITE);
  }
}

auto AntRenderer::draw_energy(const Ant& ant) const -> void {
  const auto textRect = get_coordinates_rect(ant);
  const int lineX = static_cast<int>(std::round(textRect.x));

  float energyPercentage = ant.get_energy() / STARTING_ENERGY;
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

auto AntRenderer::draw_coordinates(const Ant& ant) const -> void {
  const auto& position = ant.get_position();
  const std::string text = fmt::format("({:.2f}, {:.2f})", position.x, position.y);
  const auto coordinatesRect = get_coordinates_rect(ant);

  DrawText(text.c_str(), coordinatesRect.x, coordinatesRect.y, FONT_SIZE, BLACK);
}

auto AntRenderer::draw_direction(const Ant& ant) const -> void {
  const auto rotation = get_rotation(ant);
  const auto& bounds = ant.get_bounds();
  const auto& position = ant.get_position();

  const float lineLength = static_cast<float>(bounds.width) * 2.0F;
  const float endX = position.x + cosf(rotation * DEG2RAD) * lineLength;
  const float endY = position.y + sinf(rotation * DEG2RAD) * lineLength;

  DrawLineEx(position, {endX, endY}, LINE_THICKNESS, RED);
}

auto AntRenderer::draw_bounding(const Ant& ant) const -> void {
  const auto rotation = get_rotation(ant);
  const auto& bounds = ant.get_bounds();
  const auto& position = ant.get_position();
  const auto color = BLUE;

  DrawCircleLinesV(position, ant.get_radius(), color);
  DrawRectangleLinesExRot(bounds, position, rotation, 1.0F, color);
  DrawPixelV(position, color);
}

auto AntRenderer::get_coordinates_rect(const Ant& ant) const -> Rectangle {
  const auto& position = ant.get_position();
  const std::string text = fmt::format("({:.2f}, {:.2f})", position.x, position.y);

  Vector2 textSize = MeasureTextEx(GetFontDefault(), text.c_str(), FONT_SIZE, FONT_SPACING);
  int textX = static_cast<int>((position.x) + 1.5F - (textSize.x / 2.0F));
  int textY = static_cast<int>(((position.y) - (textSize.y / 2.0f + 20)));

  return {static_cast<float>(textX), static_cast<float>(textY), textSize.x, textSize.y};
}

auto AntRenderer::get_rotation(const Ant& ant) const -> float {
  const auto& velocity = ant.get_velocity();
  return atan2f(velocity.y, velocity.x) * RAD2DEG;
}