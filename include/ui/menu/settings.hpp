#pragma once

#include <memory>
#include <texture_cache.hpp>
#include <ui/state.hpp>

class Game;

namespace UI {
namespace Menu {
class Settings {
 public:
  Settings(UI::State& state, Game& game);
  auto draw() -> void;
  auto add_texture_cache(std::shared_ptr<TextureCache> cache) -> void;

 protected:
  std::shared_ptr<TextureCache> _textureCache;
  UI::State& _state;
  Game& _game;
};
}  // namespace Menu
}  // namespace UI
