#pragma once

#include <raylib.h>
#include <functional>
#include <optional>

class Ant;
class Population;
class TextureCache;

class AntRenderer {
 public:
  AntRenderer();
  ~AntRenderer();

  auto draw(Population& population, TextureCache& texture_cache) -> void;

 private:
  static constexpr float LINE_THICKNESS = 2.0F;
  static constexpr float FONT_SIZE = 10.0F;
  static constexpr float FONT_SPACING = 1.0F;
  static constexpr float STARTING_ENERGY = 1000.0F;

  auto draw_ant(const Ant& ant, TextureCache& texture_cache) -> void;
  auto draw_body(const Ant& ant, TextureCache& texture_cache) -> void;
  auto draw_energy(const Ant& ant) const -> void;
  auto draw_coordinates(const Ant& ant) const -> void;
  auto draw_direction(const Ant& ant) const -> void;
  auto draw_bounding(const Ant& ant) const -> void;

  [[nodiscard]] auto get_coordinates_rect(const Ant& ant) const -> Rectangle;
  [[nodiscard]] auto get_rotation(const Ant& ant) const -> float;
};