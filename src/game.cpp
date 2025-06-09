#include "game.hpp"

#include "raylib.h"
#include "raymath.h"

Game::Game() : _input(*this) {
  _camera = {.offset = Vector2Zero(), .target = Vector2Zero(), .rotation = 0.0F, .zoom = 1.0f};
  _world.get_population().set_size(50);
  _world.get_population().set_texture_path("./ant.png");
  _world.get_resources().set_food_count(100);

  SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT);
  InitWindow(_world.get_bounds().width, _world.get_bounds().height, "Neural Ants");
  SetTargetFPS(60);
}

auto Game::run() -> void {
  while (!WindowShouldClose()) {
    BeginDrawing();
    BeginMode2D(_camera);
    const float time = GetFrameTime();
    _input.update(time);
    _world.update(time);
    ClearBackground(BLACK);
    _world.draw();
    EndDrawing();
  }

  CloseWindow();
}

auto Game::get_camera() const -> const Camera2D& {
  return _camera;
}

auto Game::get_camera() -> Camera2D& {
  return _camera;
}

auto Game::set_camera(const Camera2D& camera) {
  _camera = camera;
}

auto Game::get_target_fps() const -> int {
  return _fps;
}

auto Game::set_target_fps(int fps) -> void {
  _fps = fps;
  SetTargetFPS(fps);
}