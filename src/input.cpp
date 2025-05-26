#include "input.hpp"

#include "game.hpp"

Input::Input(Game& game) : _game(game) {}

auto Input::update(float time) -> void {
  // Handle right mouse button camera pan
  if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
    _game.get_camera().offset = Vector2Add(_game.get_camera().offset, GetMouseDelta());
  }

  // Handle keyboard camera pan
  _cameraSpeed = (IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT)) ? 10.0f : 1.0f;

  const float moveSpeed = 5.0f * _cameraSpeed * time * static_cast<float>(_game.get_target_fps());

  if (IsKeyDown('A')) {
    _game.get_camera().offset.x += moveSpeed;
  }
  if (IsKeyDown('D')) {
    _game.get_camera().offset.x -= moveSpeed;
  }
  if (IsKeyDown('S')) {
    _game.get_camera().offset.y -= moveSpeed;
  }
  if (IsKeyDown('W')) {
    _game.get_camera().offset.y += moveSpeed;
  }

  // Handle keyboard camera zoom
  if (IsKeyDown(KEY_LEFT_CONTROL) || IsKeyDown(KEY_RIGHT_CONTROL)) {
    float zoomAdjust = 0.0f;
    if (IsKeyDown('=') || IsKeyDown(KEY_KP_ADD)) {
      zoomAdjust = 0.01f;
    }
    if (IsKeyDown('-') || IsKeyDown(KEY_KP_SUBTRACT)) {
      zoomAdjust = -0.01f;
    }

    if (zoomAdjust != 0.0f) {
      float oldZoom = _game.get_camera().zoom;
      _game.get_camera().zoom += zoomAdjust;

      Vector2 mousePos = GetMousePosition();
      Vector2 worldPos = GetScreenToWorld2D(mousePos, _game.get_camera());
      _game.get_camera().offset =
          Vector2Add(_game.get_camera().offset,
                     Vector2Scale(Vector2Subtract(worldPos, _game.get_camera().target),
                                  (_game.get_camera().zoom - oldZoom) / oldZoom));
    }
  }

  // Handle mouse wheel camera zoom
  auto mouseWheelMove = GetMouseWheelMove();
  if (mouseWheelMove != 0) {
    Vector2 mouseWorldPos = GetScreenToWorld2D(GetMousePosition(), _game.get_camera());
    _game.get_camera().offset = GetMousePosition();
    _game.get_camera().target = mouseWorldPos;

    float scale = 0.2f * mouseWheelMove;
    _game.get_camera().zoom = Clamp(expf(logf(_game.get_camera().zoom) + scale), 0.125f, 64.0f);
  }
}