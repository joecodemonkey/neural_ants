#pragma once
#include <imgui.h>
#include <raylib.h>

#include <memory>
#include <ui/menu/fitness_display.hpp>
#include <ui/menu/save_load.hpp>
#include <ui/menu/settings.hpp>
#include <ui/state.hpp>

#include "texture_cache.hpp"

class Game;

namespace UI {

class Renderer {
 public:
  Renderer(Game& game);
  auto draw(float deltaTime) -> void;
  auto paused() const -> bool;
  auto pause() -> void;
  auto unpause() -> void;
  auto add_texture_cache(std::shared_ptr<TextureCache> cache) -> void;

 protected:
  auto setup() -> void;

  auto draw_settings_menu() -> void;
  auto draw_settings_button() -> void;
  auto draw_settings() -> void;
  auto draw_save_load_menu() -> void;

  Game& _game;
  int _screenWidth, _screenHeight;
  bool _setup, _paused;

  UI::Menu::Settings _settingsMenu;
  UI::Menu::SaveLoad _saveLoadMenu;
  UI::Menu::FitnessDisplay _fitnessDisplay;
  std::shared_ptr<TextureCache> _textureCache;
  UI::State _state;
};
}  // namespace UI