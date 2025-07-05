#pragma once

#include <memory>
#include <string>
#include <texture_cache.hpp>
#include <ui/state.hpp>

class Game;

namespace UI {
namespace Menu {
class SaveLoad {
 public:
  SaveLoad(UI::State& state, Game& game);
  auto draw() -> void;
  auto add_texture_cache(std::shared_ptr<TextureCache> cache) -> void;

 protected:
  Game& _game;
  UI::State& _state;
  std::shared_ptr<TextureCache> _textureCache;
  std::string _statusMessage;
  float _messageTimer = 0.0f;
};
}  // namespace Menu
}  // namespace UI