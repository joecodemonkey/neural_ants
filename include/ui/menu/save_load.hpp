#pragma once

#include <functional>
#include <optional>
#include <string>
#include <texture_cache.hpp>
#include <ui/state.hpp>

class Game;

namespace UI {
namespace Menu {
class SaveLoad {
 public:
  SaveLoad(UI::State& state, Game& game, TextureCache& textureCache);
  auto draw() -> void;

 protected:
  Game& _game;
  UI::State& _state;
  TextureCache & _textureCache;
  std::string _statusMessage;
  float _messageTimer = 0.0f;
};
}  // namespace Menu
}  // namespace UI