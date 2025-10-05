#include <imgui.h>
#include <raylib.h>

#include <ui/buttons.hpp>
#include <ui/menu/settings.hpp>

#include "game.hpp"
#include "ui/state.hpp"

UI::Menu::Settings::Settings(UI::State& state, Game& game) : _state(state), _game(game) {}

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
            "#save_load", "Save/Load", _textureCache->get_texture("ui_save").id, ImVec2(50, 50))) {
      _state.minimize(State::SETTINGS);
      _state.maximize(State::SAVELOAD);
    }
    if (UI::Buttons::GroupedImage(
            "#analytics", "Analytics", _textureCache->get_texture("ui_progress").id, ImVec2(50, 50))) {
      _state.toggle(State::MEAN_FITNESS);
    }
    if (UI::Buttons::GroupedImage("#fastForward",
                                  "Faster",
                                  _textureCache->get_texture("ui_fastForward").id,
                                  ImVec2(50, 50))) {
      long long currentSpeed = _game.get_update_speed();
      _game.set_update_speed(currentSpeed * 2LL);
    }

    // Only show rewind button when speed > 1x
    if (_game.get_update_speed() > 1LL) {
      if (UI::Buttons::GroupedImage(
              "#rewind", "Slower", _textureCache->get_texture("ui_rewind").id, ImVec2(50, 50))) {
        long long newSpeed = _game.get_update_speed() / 2LL;
        if (newSpeed < 1LL) {
          newSpeed = 1LL;
        }
        _game.set_update_speed(newSpeed);
      }
    }

    ImGui::SetCursorPos(ImVec2{10.0f, screenHeight - 80.0f});

    if (UI::Buttons::GroupedImage(
            "#exitSettings", "Return", _textureCache->get_texture("ui_exit").id, ImVec2(50, 50))) {
      _state.minimize(State::SETTINGS);
    }
  }
  ImGui::End();
}

auto UI::Menu::Settings::add_texture_cache(std::shared_ptr<TextureCache> cache) -> void {
  _textureCache = cache;
}
