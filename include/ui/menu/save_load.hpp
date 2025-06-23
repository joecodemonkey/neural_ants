#include <texture_cache.hpp>
#include <ui/behaviors/maximizable.hpp>

namespace UI {
namespace Menu {
class SaveLoad {
 public:
  auto draw() -> void;
  auto settingsMaximizer() -> Behaviors::Maximizable&;
  auto settingsMaximizer() const -> const Behaviors::Maximizable&;
  auto maximizer() -> Behaviors::Maximizable&;
  auto maximizer() const -> const Behaviors::Maximizable&;
  auto add_texture_cache(std::shared_ptr<TextureCache> cache) -> void;

 protected:
  UI::Behaviors::Maximizable _maximizer;
  UI::Behaviors::Maximizable _settingsMaximizer;
  std::shared_ptr<TextureCache> _textureCache;
};
}  // namespace Menu
}  // namespace UI