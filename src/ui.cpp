#include <imgui.h>
#include <raylib.h>
#include <rlImGui.h>

#include <algorithm>
#include <cstdint>
#include <ui.hpp>

bool GroupedImageButton(const char* id,
                        const char* label,
                        ImTextureID textureID,
                        ImVec2 imageSize) {
  ImGui::BeginGroup();

  bool clicked = ImGui::ImageButton(id, textureID, imageSize);

  // Center text under button
  ImVec2 textSize = ImGui::CalcTextSize(label);
  float textOffset = (imageSize.x - textSize.x) * 0.5f;
  if (textOffset > 0) {
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + textOffset);
  }
  ImGui::Text("%s", label);

  ImGui::EndGroup();

  return clicked;
}

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
  _exitTexture = LoadTexture("exitRight.png");
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
  const auto settingsButtonPosition = ImVec2{_screenWidth - 80.0f, _screenHeight - 85.0f};
  static const ImGuiWindowFlags buttonWindowFlags =
      ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
      ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoSavedSettings |
      ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoDecoration;

  // Set panel position and size
  ImGui::SetNextWindowPos(settingsButtonPosition);

  if (ImGui::Begin("Right Panel", nullptr, buttonWindowFlags)) {
    begin_button_style();

    if (ImGui::ImageButton("##settings", _gearTexture.id, ImVec2(50, 50))) {
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
  float settingsWindowWidth = 80.0f;
  float settingsWindowHeight = _screenHeight;
  const auto settingsWindowPosition =
      ImVec2{(_screenWidth - settingsWindowWidth), (_screenHeight - settingsWindowHeight) * 0.5f};

  ImGui::SetNextWindowPos(settingsWindowPosition);
  ImGui::SetNextWindowSize(ImVec2(settingsWindowWidth, settingsWindowHeight));

  if (ImGui::Begin("Settings", &_settingsMaximized, settingsWindowFlags)) {
    // Draw the top bar
    ImDrawList* draw_list = ImGui::GetWindowDrawList();

    // Add close button in top right corner
    begin_button_style();

    GroupedImageButton("#save_load", "Save/Load", _saveButtonTexture.id, ImVec2(50, 50));
    GroupedImageButton("#progress", " Enable\nProgress", _progressTexture.id, ImVec2(50, 50));

    ImGui::SetCursorPos(ImVec2{10.0f, _screenHeight - 80.0f});

    if (GroupedImageButton("#exitSettings", "Return", _exitTexture.id, ImVec2(50, 50))) {
      _settingsMaximized = false;
    }
    end_button_style();
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

auto UI::begin_button_style() -> void {
  ImGui::PushStyleColor(ImGuiCol_Button, _buttonColor);
  ImGui::PushStyleColor(ImGuiCol_ButtonHovered, _buttonColor);
  ImGui::PushStyleColor(ImGuiCol_ButtonActive, _buttonColor);
}
auto UI::end_button_style() -> void {
  ImGui::PopStyleColor(3);
}
