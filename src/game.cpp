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
  bool texturesLoaded = false;
  while (!WindowShouldClose()) {
    BeginDrawing();
    if (!texturesLoaded) {
      load_textures();
      _ui.add_texture_cache(_textureCache);
      texturesLoaded = true;
    }
    BeginMode2D(_camera);
    const float time = GetFrameTime();
    if (!_ui.paused()) {
      _input.update(time);
      _world.update(time);
    }
    ClearBackground(BLACK);
    _world.draw();
    EndMode2D();
    _ui.draw(time);
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

auto Game::load_textures() -> void {
  _textureCache = std::make_shared<TextureCache>();
  if (!_textureCache->add_texture("close", "buttonX.png")) {
    throw std::runtime_error("Failed to load default texture: 'buttonX.png'");
  }
  _textureCache->set_default("close");
  _textureCache->add_texture("settings", "gear.png");
  _textureCache->add_texture("save", "save.png");
  _textureCache->add_texture("delete", "trashcan.png");
  _textureCache->add_texture("load", "import.png");
  _textureCache->add_texture("progress", "signal3.png");
  _textureCache->add_texture("exit", "exitRight.png");
  _textureCache->add_texture("ant", "default");
}