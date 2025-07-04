#include <imgui.h>
#include <raylib.h>
#include <rlImGui.h>

#include <expected>
#include <string>
#include <fmt/format.h>
#include <ui/renderer.hpp>

#include "game.hpp"
#include "ui/buttons.hpp"
#include "ui/state.hpp"

UI::Renderer::Renderer(Game& game)
    : _game(game),
      _setup(false),
      _paused(false),
      _settingsMenu(_state, game),
      _saveLoadMenu(_state, game) {}

auto UI::Renderer::setup() -> void {
  if (_setup) {
    return;
  }
  rlImGuiSetup(false);

  _settingsMenu.add_texture_cache(_textureCache);
  _saveLoadMenu.add_texture_cache(_textureCache);
  _setup = true;

  ImGuiStyle& style = ImGui::GetStyle();

  ImVec4* colors = style.Colors;

  // Base Colors (from Godot Dark Theme)
  colors[ImGuiCol_Text] = ImVec4(0.88f, 0.88f, 0.88f, 1.00f);
  colors[ImGuiCol_TextDisabled] = ImVec4(0.48f, 0.48f, 0.48f, 1.00f);
  colors[ImGuiCol_WindowBg] = ImVec4(0.17f, 0.17f, 0.17f, 1.00f);
  colors[ImGuiCol_ChildBg] = ImVec4(0.23f, 0.23f, 0.23f, 1.00f);
  colors[ImGuiCol_PopupBg] = ImVec4(0.20f, 0.20f, 0.20f, 0.95f);
  colors[ImGuiCol_Border] = ImVec4(0.30f, 0.30f, 0.30f, 0.60f);
  colors[ImGuiCol_FrameBg] = ImVec4(0.27f, 0.27f, 0.27f, 1.00f);
  colors[ImGuiCol_FrameBgHovered] = ImVec4(0.36f, 0.36f, 0.36f, 1.00f);
  colors[ImGuiCol_FrameBgActive] = ImVec4(0.43f, 0.43f, 0.43f, 1.00f);

  colors[ImGuiCol_Button] = ImVec4(0.27f, 0.27f, 0.27f, 1.00f);
  colors[ImGuiCol_ButtonHovered] = ImVec4(0.36f, 0.36f, 0.36f, 1.00f);
  colors[ImGuiCol_ButtonActive] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);

  colors[ImGuiCol_Header] = ImVec4(0.32f, 0.47f, 0.71f, 0.80f);
  colors[ImGuiCol_HeaderHovered] = ImVec4(0.32f, 0.47f, 0.71f, 1.00f);
  colors[ImGuiCol_HeaderActive] = ImVec4(0.29f, 0.42f, 0.63f, 1.00f);

  colors[ImGuiCol_SliderGrab] = ImVec4(0.40f, 0.60f, 0.90f, 1.00f);
  colors[ImGuiCol_SliderGrabActive] = ImVec4(0.30f, 0.50f, 0.85f, 1.00f);
  colors[ImGuiCol_ScrollbarBg] = ImVec4(0.20f, 0.20f, 0.20f, 0.50f);
  colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.40f, 0.40f, 0.40f, 1.00f);
  colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
  colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);

  colors[ImGuiCol_CheckMark] = ImVec4(0.32f, 0.47f, 0.71f, 1.00f);

  colors[ImGuiCol_Tab] = ImVec4(0.23f, 0.23f, 0.23f, 1.00f);
  colors[ImGuiCol_TabHovered] = ImVec4(0.32f, 0.47f, 0.71f, 1.00f);
  colors[ImGuiCol_TabActive] = ImVec4(0.27f, 0.45f, 0.70f, 1.00f);

  colors[ImGuiCol_TitleBg] = ImVec4(0.23f, 0.23f, 0.23f, 1.00f);
  colors[ImGuiCol_TitleBgActive] = ImVec4(0.27f, 0.45f, 0.70f, 1.00f);
  colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.17f, 0.17f, 0.17f, 1.00f);
  colors[ImGuiCol_TableHeaderBg] = colors[ImGuiCol_TitleBg];
  colors[ImGuiCol_TableBorderStrong] = colors[ImGuiCol_FrameBgHovered];

  style.WindowRounding = 2.0f;
  style.FrameRounding = 2.0f;
  style.ScrollbarRounding = 2.0f;
}

auto UI::Renderer::draw(float deltaTime) -> void {
  setup();
  
  _screenHeight = GetScreenHeight();
  _screenWidth = GetScreenWidth();
  
  draw_speed_display();
  
  rlImGuiBegin();
  if (_state.is_maximized(State::SETTINGS)) {
    _paused = false;
    _settingsMenu.draw();
  } else if (_state.is_maximized(State::SAVELOAD)) {
    _paused = true;
    _saveLoadMenu.draw();
  } else {
    _paused = false;
    draw_settings_button();
  }
  rlImGuiEnd();
}

auto UI::Renderer::draw_settings_button() -> void {
  const auto settingsButtonPosition = ImVec2{_screenWidth - 80.0f, _screenHeight - 85.0f};
  static const ImGuiWindowFlags buttonWindowFlags =
      ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
      ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoSavedSettings |
      ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoDecoration;

  // Set panel position and size
  ImGui::SetNextWindowPos(settingsButtonPosition);

  if (ImGui::Begin("Right Panel", nullptr, buttonWindowFlags)) {
    UI::Buttons::begin_button_style();

    if (ImGui::ImageButton(
            "##settings", _textureCache->get_texture("settings").id, ImVec2(50, 50))) {
      _state.maximize(State::SETTINGS);
    }
    UI::Buttons::end_button_style();
  }
  ImGui::End();
}

auto UI::Renderer::add_texture_cache(std::shared_ptr<TextureCache> cache) -> void {
  _textureCache = cache;
}

auto UI::Renderer::paused() const -> bool {
  return _paused;
}

auto UI::Renderer::pause() -> void {
  _paused = true;
}

auto UI::Renderer::unpause() -> void {
  _paused = false;
}

auto UI::Renderer::draw_speed_display() -> void {
  static long long lastUpdateSpeed = 1LL;
  static float displayTimer = 0.0f;
  static bool showSpeedDisplay = false;
  
  long long currentSpeed = _game.get_update_speed();
  
  if (currentSpeed != lastUpdateSpeed) {
    lastUpdateSpeed = currentSpeed;
    displayTimer = 3.0f;
    showSpeedDisplay = true;
  }
  
  if (showSpeedDisplay && displayTimer > 0.0f) {
    displayTimer -= GetFrameTime();
    
    if (displayTimer <= 0.0f) {
      showSpeedDisplay = false;
    } else {
      std::string speedText = fmt::format("Speed: {}x", currentSpeed);
      int fontSize = 40;
      int textWidth = MeasureText(speedText.c_str(), fontSize);
      
      int centerX = _screenWidth / 2 - textWidth / 2;
      int centerY = _screenHeight / 2 - fontSize / 2;
      
      DrawText(speedText.c_str(), centerX, centerY, fontSize, RED);
    }
  }
}
