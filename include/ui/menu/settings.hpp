#pragma once

#include <functional>
#include <optional>
#include <texture_cache.hpp>
#include <ui/state.hpp>

class Game;

namespace UI {
namespace Menu {
class Settings {
 public:
  Settings(UI::State& state, Game& game, TextureCache& textureCache);
  auto draw() -> void;

 protected:
  TextureCache &_textureCache;
  UI::State& _state;
  Game& _game;
};
}
}
