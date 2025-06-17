#include "ui.hpp"

#include <algorithm>

#include "imgui.h"
#include "raylib.h"
#include "rlImGui.h"

auto UI::setup() -> void {
  _screenHeight = GetScreenHeight();
  _screenWidth = GetScreenWidth();

  if (_setup) {
    return;
  }
  rlImGuiSetup(false);
  _gearTexture = LoadTexture("gear.png");
  _setup = true;
}

auto UI::update(float deltaTime) -> void {
  setup();
  rlImGuiBegin();

  update_settings_panel(deltaTime);
  rlImGuiEnd();
}

auto UI::draw_settings_button(float deltaTime) -> void {
  ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
                                 ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar |
                                 ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoBackground |
                                 ImGuiWindowFlags_NoDecoration;

  auto buttonPosition = ImVec2{0, 0};
  buttonPosition.x = _screenWidth - 50.0f;
  buttonPosition.y = _screenHeight - 50.0f;

  // Set panel position and size
  ImGui::SetNextWindowPos(buttonPosition);

  if (ImGui::Begin("Right Panel", nullptr, windowFlags)) {
    // Add image button at the top with periwinkle color
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.8f, 0.8f, 0.9f, 1.0f));  // Periwinkle color
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered,
                          ImVec4(0.9f, 0.9f, 1.0f, 1.0f));  // Lighter periwinkle on hover

    if (ImGui::ImageButton("##settings", (ImTextureID)(uintptr_t)_gearTexture.id, ImVec2(30, 30))) {
      _settingsMaximized = true;
    }
    ImGui::PopStyleColor(2);  // Pop both color styles
  }
  ImGui::End();
}

auto UI::update_settings_panel(float deltaTime) -> void {
  if (_settingsMaximized) {
  } else {
    draw_settings_button(deltaTime);
  }
}