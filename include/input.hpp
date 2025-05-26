#pragma once

#include <raylib.h>
#include <raymath.h>

class Game;

class Input {
 public:
  Input(Game& game);
  ~Input() = default;
  auto update(float time) -> void;

 private:
  Game& _game;
  float _cameraSpeed = 1.0f;
};