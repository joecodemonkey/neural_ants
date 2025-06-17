#pragma once
#include "raylib.h"

class UI {
 public:
  UI() = default;
  auto update(float deltaTime) -> void;

 protected:
  bool _setup = false;
  auto setup() -> void;

  auto update_settings_panel(float deltaTime) -> void;
  auto draw_settings_button(float deltaTime) -> void;

  int _screenWidth;
  int _screenHeight;

  bool _settingsMaximized = false;
  Texture2D _gearTexture;
};