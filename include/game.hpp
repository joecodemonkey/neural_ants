#pragma once

#include <raylib.h>
#include <raymath.h>

#include <ui/renderer.hpp>

#include "input.hpp"
#include "texture_cache.hpp"
#include "world.hpp"

class Game {
 public:
  Game();
  auto run() -> void;

  auto get_camera() const -> const Camera2D&;
  auto get_camera() -> Camera2D&;
  auto set_camera(const Camera2D& camera);
  auto get_target_fps() const -> int;
  auto set_target_fps(int fps) -> void;

 private:
  auto load_textures() -> void;
  const float DEFAULT_FPS = 60;
  Camera2D _camera;
  World _world;
  Input _input;
  float _cameraSpeed;
  int _fps = DEFAULT_FPS;
  UI::Renderer _ui;
  std::shared_ptr<TextureCache> _textureCache;
};
