#include <imgui.h>
#include <raylib.h>

#include <ui/buttons.hpp>
#include <ui/menu/settings.hpp>

auto UI::Menu::Settings::maximizer() -> UI::Behaviors::Maximizable& {
  return _maximizer;
}
auto UI::Menu::Settings::maximizer() const -> const UI::Behaviors::Maximizable& {
  return _maximizer;
}

auto UI::Menu::Settings::saveLoadMaximizer() -> UI::Behaviors::Maximizable& {
  return _saveLoadMaximizer;
}
auto UI::Menu::Settings::saveLoadMaximizer() const -> const UI::Behaviors::Maximizable& {
  return _saveLoadMaximizer;
}

auto UI::Menu::Settings::progressMaximizer() -> UI::Behaviors::Maximizable& {
  return _progressMaximizer;
}
auto UI::Menu::Settings::progressMaximizer() const -> const UI::Behaviors::Maximizable& {
  return _progressMaximizer;
}

auto UI::Menu::Settings::draw() -> void {
  if (!_maximizer.maximized()) {
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

  bool maximized = _maximizer.maximized();

  if (ImGui::Begin("Settings", &maximized, settingsWindowFlags)) {
    if (UI::Buttons::GroupedImage(
            "#save_load", "Save/Load", _textureCache->get_texture("save").id, ImVec2(50, 50))) {
      _saveLoadMaximizer.maximize();
      _maximizer.minimize();
    }
    if (UI::Buttons::GroupedImage("#progress",
                                  " Enable\nProgress",
                                  _textureCache->get_texture("progress").id,
                                  ImVec2(50, 50))) {
      _progressMaximizer.maximize();
    }

    ImGui::SetCursorPos(ImVec2{10.0f, screenHeight - 80.0f});

    if (UI::Buttons::GroupedImage(
            "#exitSettings", "Return", _textureCache->get_texture("exit").id, ImVec2(50, 50))) {
      _maximizer.minimize();
      _saveLoadMaximizer.maximize();
    }
  }
  ImGui::End();
}

auto UI::Menu::Settings::add_texture_cache(std::shared_ptr<TextureCache> cache) -> void {
  _textureCache = cache;
}
