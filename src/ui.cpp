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
  _setup = true;
}
auto UI::update(float deltaTime) -> void {
  setup();
  rlImGuiBegin();

  update_right_panel(deltaTime);
  rlImGuiEnd();
}

auto UI::update_right_panel(float deltaTime) -> void {
  ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
                                 ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar |
                                 ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoBackground |
                                 ImGuiWindowFlags_NoDecoration;

  // Calculate panel width based on screen width
  float panelWidth = std::min(100.0f, std::max(50.0f, _screenWidth * 0.1f));

  // Set panel position and size
  ImGui::SetNextWindowPos(ImVec2(_screenWidth - panelWidth, 0));
  ImGui::SetNextWindowSize(ImVec2(panelWidth, _screenHeight));

  if (ImGui::Begin("RightPanel", nullptr, windowFlags)) {
    // Add push button at the top with periwinkle color
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.8f, 0.8f, 0.9f, 1.0f));  // Periwinkle color
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered,
                          ImVec4(0.9f, 0.9f, 1.0f, 1.0f));  // Lighter periwinkle on hover
    if (ImGui::Button("push", ImVec2(panelWidth - 20, 30))) {
      // Button does nothing as specified
    }
    ImGui::PopStyleColor(2);  // Pop both color styles
  }
  ImGui::End();
}