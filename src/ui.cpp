#include <rlImGui.h>

#include <algorithm>
#include <cstdint>
#include <ui.hpp>

#include "imgui.h"
#include "raylib.h"

UI::UI()
    : _buttonColor{0.2f, 0.2f, 0.2f, 1.0f},
      _buttonHoverColor{0.4f, 0.4f, 0.4f, 1.0f},
      _activeButtonColor{0.3f, 0.3f, 0.3f, 1.0f},
      _setup(false),
      _settingsMaximized(false) {}

auto UI::setup() -> void {
  if (_setup) {
    return;
  }
  rlImGuiSetup(false);
  _gearTexture = LoadTexture("gear.png");
  _closeButtonTexture = LoadTexture("buttonX.png");
  _saveButtonTexture = LoadTexture("save.png");
  _deleteTexture = LoadTexture("trashcan.png");
  _loadTexture = LoadTexture("ant.png");
  _progressTexture = LoadTexture("signal3.png");
  _setup = true;
}

auto UI::draw(float deltaTime) -> void {
  setup();
  rlImGuiBegin();

  _screenHeight = GetScreenHeight();
  _screenWidth = GetScreenWidth();
  _deltaTime = deltaTime;

  draw_settings();
  rlImGuiEnd();
}

auto UI::draw_settings_button() -> void {
  const auto settingsButtonPosition = ImVec2{_screenWidth - 50.0f, _screenHeight - 50.0f};
  static const ImGuiWindowFlags buttonWindowFlags =
      ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
      ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoSavedSettings |
      ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoDecoration;

  // Set panel position and size
  ImGui::SetNextWindowPos(settingsButtonPosition);

  if (ImGui::Begin("Right Panel", nullptr, buttonWindowFlags)) {
    begin_button_style();

    if (ImGui::ImageButton("##settings", (ImTextureID)(uintptr_t)_gearTexture.id, ImVec2(30, 30))) {
      _settingsMaximized = true;
    }
    end_button_style();
  }
  ImGui::End();
}

auto UI::draw_settings_menu() -> void {
  static const ImGuiWindowFlags settingsWindowFlags =
      ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
      ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoTitleBar;
  // Calculate center position for the panel
  float settingsWindowWidth = std::max(0.4f * _screenWidth, 100.0f);
  float settingsWindowHeight = std::max(0.4f * _screenHeight, 100.0f);
  const auto settingsWindowPosition = ImVec2{(_screenWidth - settingsWindowWidth) * 0.5f,
                                             (_screenHeight - settingsWindowHeight) * 0.5f};

  ImGui::SetNextWindowPos(settingsWindowPosition);
  ImGui::SetNextWindowSize(ImVec2(settingsWindowWidth, settingsWindowHeight));

  if (ImGui::Begin("Settings", &_settingsMaximized, settingsWindowFlags)) {
    // Draw the top bar
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    const ImVec2 window_pos = ImGui::GetWindowPos();
    draw_list->AddRectFilled(ImVec2(window_pos.x, window_pos.y),
                             ImVec2(window_pos.x + settingsWindowWidth, window_pos.y + 40),
                             ImGui::ColorConvertFloat4ToU32(_buttonColor));

    // Add close button in top right corner
    ImGui::SetCursorPos(ImVec2(settingsWindowWidth - 40, 0));
    begin_button_style();

    if (ImGui::ImageButton(
            "Close", (ImTextureID)(uintptr_t)_closeButtonTexture.id, ImVec2(30, 30))) {
      _settingsMaximized = false;
    }
    end_button_style();
    draw_save_load_button();
    draw_leaderboard_button();
  }

  ImGui::End();
}

auto UI::draw_save_load_button() -> void {
  ImGui::SetCursorPos(ImVec2(10, 50));  // Relative to window content area

  begin_button_style();
  if (ImGui::Button("Save/Load", ImVec2(100, 30))) {
    // Handle save/load
  }
  end_button_style();
}

auto UI::draw_leaderboard_button() -> void {
  // You can also use relative positioning
  begin_button_style();
  ImGui::SetCursorPos(ImVec2(10, 100));
  if (ImGui::Button("Leaderboard", ImVec2(100, 30))) {
  }
  end_button_style();
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

auto UI::begin_button_style() -> void {
  ImGui::PushStyleColor(ImGuiCol_Button, _buttonColor);
  ImGui::PushStyleColor(ImGuiCol_ButtonHovered, _buttonColor);
  ImGui::PushStyleColor(ImGuiCol_ButtonActive, _buttonColor);
}
auto UI::end_button_style() -> void {
  ImGui::PopStyleColor(3);
}
