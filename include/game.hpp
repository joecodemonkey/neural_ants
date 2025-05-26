#pragma once

#include <raylib.h>
#include <raymath.h>

#include "input.hpp"
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
  const float DEFAULT_FPS = 60;
  Camera2D _camera;
  World _world;
  Input _input;
  float _cameraSpeed;
  int _fps = DEFAULT_FPS;
};
