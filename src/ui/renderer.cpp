#include <imgui.h>
#include <raylib.h>
#include <rlImGui.h>

#include <expected>
#include <ui/renderer.hpp>

#include "ui/buttons.hpp"

UI::Renderer::Renderer() : _setup(false), _paused(false) {}

auto UI::Renderer::setup() -> void {
  if (_setup) {
    return;
  }
  rlImGuiSetup(false);

  _settingsMenu.add_texture_cache(_textureCache);
  _saveLoadMenu.add_texture_cache(_textureCache);
  _setup = true;
}

auto UI::Renderer::draw(float deltaTime) -> void {
  setup();
  rlImGuiBegin();

  _screenHeight = GetScreenHeight();
  _screenWidth = GetScreenWidth();
  if (_settingsMenu.maximizer().maximized()) {
    _settingsMenu.draw();
    _saveLoadMenu.maximizer() = _settingsMenu.saveLoadMaximizer();
  } else if (_saveLoadMenu.maximizer().maximized()) {
    _saveLoadMenu.draw();
    // TODO: GET RID OF MAXIMIZER
    _settingsMenu.maximizer() = _saveLoadMenu.settingsMaximizer();
    _settingsMenu.saveLoadMaximizer() = _saveLoadMenu.maximizer();
  } else {
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
      _settingsMenu.maximizer().maximize();
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
