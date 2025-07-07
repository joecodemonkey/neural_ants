#pragma once

#include <raylib.h>

class Ant;
class Population;
class TextureCache;

class AntRenderer {
 public:
  AntRenderer();
  ~AntRenderer();

  auto set_texture_cache(TextureCache* texture_cache) -> void;
  auto set_scale(float scale) -> void;
  auto draw(Population& population) -> void;

 private:
  static constexpr float LINE_THICKNESS = 2.0F;
  static constexpr float FONT_SIZE = 10.0F;
  static constexpr float FONT_SPACING = 1.0F;
  static constexpr float DEFAULT_SCALE = 20.0F;
  static constexpr float STARTING_ENERGY = 1000.0F;

  TextureCache* _textureCache = nullptr;
  Texture2D _antTexture;
  float _scale = DEFAULT_SCALE;
  bool _textureLoaded = false;

  auto load_ant_texture() -> bool;
  auto is_texture_valid() const -> bool;

  auto draw_ant(const Ant& ant) -> void;
  auto draw_body(const Ant& ant) -> void;
  auto draw_energy(const Ant& ant) const -> void;
  auto draw_coordinates(const Ant& ant) const -> void;
  auto draw_direction(const Ant& ant) const -> void;
  auto draw_bounding(const Ant& ant) const -> void;

  [[nodiscard]] auto get_coordinates_rect(const Ant& ant) const -> Rectangle;
  [[nodiscard]] auto get_rotation(const Ant& ant) const -> float;
};