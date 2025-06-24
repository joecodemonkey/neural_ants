#include <texture_cache.hpp>
#include <ui/state.hpp>

namespace UI {
namespace Menu {
class SaveLoad {
 public:
  SaveLoad(UI::State& state);
  auto draw() -> void;
  auto add_texture_cache(std::shared_ptr<TextureCache> cache) -> void;

 protected:
  UI::State& _state;
  std::shared_ptr<TextureCache> _textureCache;
};
}  // namespace Menu
}  // namespace UI