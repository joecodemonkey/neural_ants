#include <texture_cache.hpp>
#include <ui/state.hpp>

namespace UI {
namespace Menu {
class Settings {
 public:
  Settings(UI::State& state);
  auto draw() -> void;
  auto add_texture_cache(std::shared_ptr<TextureCache> cache) -> void;

 protected:
  std::shared_ptr<TextureCache> _textureCache;
  UI::State& _state;
};
}  // namespace Menu
}  // namespace UI
