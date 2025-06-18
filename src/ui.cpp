#include "ui.hpp"

#include <cstdint>
#include <iostream>

#include "imgui.h"
#include "raylib.h"
#include "rlImGui.h"

auto UI::setup() -> void {
  rlImGuiSetup(false);
  _gearTexture = LoadTexture("gear.png");
  _closeButtonTexture = LoadTexture("buttonX.png");
  _setup = true;
  _settingsMaximized = false;
  _buttonWindowFlags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
                       ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar |
                       ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoBackground |
                       ImGuiWindowFlags_NoDecoration;

  _settingsWindowFlags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize |
                         ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar |
                         ImGuiWindowFlags_NoTitleBar;

  _buttonColor = ImVec4(0.2f, 0.2f, 0.2f, 1.0f);
  _buttonHoverColor = ImVec4(0.4f, 0.4f, 0.4f, 1.0f);
  _activeButtonColor = ImVec4(0.3f, 0.3f, 0.3f, 1.0f);
}

auto UI::update(float deltaTime) -> void {
  if (!_setup)
    setup();

  _screenHeight = GetScreenHeight();
  _screenWidth = GetScreenWidth();

  update_settings(deltaTime);
}

auto UI::update_settings(float deltaTime) -> void {
  if (_settingsMaximized) {
    return update_settings_menu(deltaTime);
  } else {
    update_settings_button(deltaTime);
  }
}

auto UI::update_settings_button(float deltaTime) -> void {
  _settingsButtonPosition = ImVec2{_screenWidth - 50.0f, _screenHeight - 50.0f};
}

auto UI::update_settings_menu(float deltaTime) -> void {
  // Calculate center position for the panel
  _settingsWindowWidth = 400.0f;
  _settingsWindowHeight = 300.0f;
  _settingsWindowPosition = ImVec2{(_screenWidth - _settingsWindowWidth) * 0.5f,
                                   (_screenHeight - _settingsWindowHeight) * 0.5f};
}

auto UI::draw() -> void {
  rlImGuiBegin();
  draw_settings();
  rlImGuiEnd();
}

auto UI::draw_settings_button() -> void {
  // Set panel position and size
  ImGui::SetNextWindowPos(_settingsButtonPosition);

  if (ImGui::Begin("Right Panel", nullptr, _buttonWindowFlags)) {
    // Add image button at the top with dark grey color
    ImGui::PushStyleColor(ImGuiCol_Button, _buttonColor);
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, _buttonHoverColor);
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, _activeButtonColor);

    if (ImGui::ImageButton("##settings", (ImTextureID)(uintptr_t)_gearTexture.id, ImVec2(30, 30))) {
      _settingsMaximized = true;
    }
    ImGui::PopStyleColor(3);  // Pop both color styles
  }
  ImGui::End();
}

auto UI::draw_settings_menu() -> void {
  ImGui::SetNextWindowPos(_settingsWindowPosition);
  ImGui::SetNextWindowSize(ImVec2(_settingsWindowWidth, _settingsWindowHeight));

  if (ImGui::Begin("Settings", &_settingsMaximized, _settingsWindowFlags)) {
    // Draw the top bar
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    ImVec2 window_pos = ImGui::GetWindowPos();
    draw_list->AddRectFilled(ImVec2(window_pos.x, window_pos.y),
                             ImVec2(window_pos.x + _settingsWindowWidth, window_pos.y + 40),
                             ImGui::ColorConvertFloat4ToU32(_buttonColor));

    // Add close button in top right corner
    ImGui::SetCursorPos(ImVec2(_settingsWindowWidth - 40, 0));
    ImGui::PushStyleColor(ImGuiCol_Button, _buttonColor);
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, _buttonColor);
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, _buttonColor);

    if (ImGui::ImageButton(
            "##close", (ImTextureID)(uintptr_t)_closeButtonTexture.id, ImVec2(30, 30))) {
      _settingsMaximized = false;
    }
    ImGui::PopStyleColor(3);
  }
  ImGui::End();
}

auto UI::draw_settings() -> void {
  if (_settingsMaximized) {
    draw_settings_menu();
  } else {
    draw_settings_button();
  }
}

auto UI::get_settings_maximized() const -> bool {
  return _settingsMaximized;
}