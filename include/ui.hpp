#pragma once
#include <imgui.h>
#include <raylib.h>

class UI {
 public:
  UI();
  auto draw(float deltaTime) -> void;
  auto get_settings_maximized() const -> bool;

 protected:
  auto setup() -> void;

  auto draw_settings_menu() -> void;
  auto draw_settings_button() -> void;
  auto draw_settings() -> void;
  auto draw_save_load_button() -> void;
  auto draw_leaderboard_button() -> void;
  auto begin_button_style() -> void;
  auto end_button_style() -> void;

  int _screenWidth, _screenHeight;
  bool _settingsMaximized, _setup;
  float _deltaTime;

  Texture2D _gearTexture, _saveButtonTexture, _closeButtonTexture;
  Texture2D _deleteTexture, _loadTexture, _progressTexture;
  const ImVec4 _buttonColor, _buttonHoverColor, _activeButtonColor;
};