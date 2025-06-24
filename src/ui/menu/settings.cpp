#include <imgui.h>
#include <raylib.h>

#include <ui/buttons.hpp>
#include <ui/menu/settings.hpp>

#include "ui/state.hpp"

UI::Menu::Settings::Settings(UI::State& state) : _state(state) {}

auto UI::Menu::Settings::draw() -> void {
  if (!_state.is_maximized(State::SETTINGS)) {
    return;
  }

  static const ImGuiWindowFlags settingsWindowFlags =
      ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
      ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoTitleBar;

  const auto screenHeight = GetScreenHeight();
  const auto screenWidth = GetScreenWidth();

  float settingsWindowWidth = 80.0f;
  float settingsWindowHeight = screenHeight;
  const auto settingsWindowPosition =
      ImVec2{(screenWidth - settingsWindowWidth), (screenHeight - settingsWindowHeight) * 0.5f};

  ImGui::SetNextWindowPos(settingsWindowPosition);
  ImGui::SetNextWindowSize(ImVec2(settingsWindowWidth, settingsWindowHeight));

  bool maximized = true;

  if (ImGui::Begin("Settings", &maximized, settingsWindowFlags)) {
    if (UI::Buttons::GroupedImage(
            "#save_load", "Save/Load", _textureCache->get_texture("save").id, ImVec2(50, 50))) {
      _state.minimize(State::SETTINGS);
      _state.maximize(State::SAVELOAD);
    }
    if (UI::Buttons::GroupedImage("#progress",
                                  " Enable\nProgress",
                                  _textureCache->get_texture("progress").id,
                                  ImVec2(50, 50))) {
      _state.maximize(State::PROGRESS);
    }

    ImGui::SetCursorPos(ImVec2{10.0f, screenHeight - 80.0f});

    if (UI::Buttons::GroupedImage(
            "#exitSettings", "Return", _textureCache->get_texture("exit").id, ImVec2(50, 50))) {
      _state.minimize(State::SETTINGS);
    }
  }
  ImGui::End();
}

auto UI::Menu::Settings::add_texture_cache(std::shared_ptr<TextureCache> cache) -> void {
  _textureCache = cache;
}
