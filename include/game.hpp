#pragma once

#include <raylib.h>
#include <raymath.h>

#include <expected>
#include <input.hpp>
#include <string>
#include <texture_cache.hpp>
#include <ui/renderer.hpp>
#include <world.hpp>
#include <ant_renderer.hpp>

class Game {
 public:
  Game();
  auto run() -> void;

  auto get_camera() const -> const Camera2D&;
  auto get_camera() -> Camera2D&;
  auto set_camera(const Camera2D& camera) -> void;
  auto get_world() const -> const World&;
  auto get_world() -> World&;
  auto get_target_fps() const -> int;
  auto set_target_fps(int fps) -> void;
  auto get_update_speed() const -> long long;
  auto set_update_speed(long long speed) -> void;
  auto get_texture_cache() -> std::shared_ptr<TextureCache>;

  auto save_game(const std::string& filename) const -> std::expected<void, std::string>;
  auto load_game(const std::string& filename) -> std::expected<void, std::string>;
  auto delete_save(const std::string& filename) -> std::expected<void, std::string>;

 private:
  auto load_textures() -> void;
  auto initialize_raylib() -> void;
  auto no_render_run() -> void;
  const float DEFAULT_FPS = 60;
  Camera2D _camera;
  World _world;
  Input _input;
  float _cameraSpeed;
  int _fps = DEFAULT_FPS;
  long long _updateSpeed;
  float _lastSpeedAdjustmentTime = 0.0f;
  bool _raylibInitialized = false;
  UI::Renderer _ui;
  std::shared_ptr<TextureCache> _textureCache;
  AntRenderer _antRenderer;
};
