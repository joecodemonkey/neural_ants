#pragma once
#include "imgui.h"
#include "raylib.h"

class UI {
 public:
  UI() = default;
  auto update(float deltaTime) -> void;
  auto draw() -> void;
  auto get_settings_maximized() const -> bool;

 protected:
  bool _setup = false;
  auto setup() -> void;

  auto update_settings(float deltaTime) -> void;
  auto update_settings_button(float deltaTime) -> void;
  auto update_settings_menu(float deltaTime) -> void;

  auto draw_settings_menu() -> void;
  auto draw_settings_button() -> void;
  auto draw_settings() -> void;

  int _screenWidth;
  int _screenHeight;

  bool _settingsMaximized = false;
  Texture2D _gearTexture;
  Texture2D _closeButtonTexture;
  ImVec2 _settingsButtonPosition;
  ImGuiWindowFlags _buttonWindowFlags, _settingsWindowFlags;

  ImVec4 _buttonColor;
  ImVec4 _buttonHoverColor;
  ImVec4 _activeButtonColor;

  ImVec2 _settingsWindowPosition;
  float _settingsWindowHeight;
  float _settingsWindowWidth;
};