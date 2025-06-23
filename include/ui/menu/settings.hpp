#include <texture_cache.hpp>
#include <ui/behaviors/maximizable.hpp>

namespace UI {
namespace Menu {
class Settings {
 public:
  auto draw() -> void;
  auto maximizer() -> Behaviors::Maximizable&;
  auto maximizer() const -> const Behaviors::Maximizable&;
  auto saveLoadMaximizer() -> Behaviors::Maximizable&;
  auto saveLoadMaximizer() const -> const Behaviors::Maximizable&;
  auto progressMaximizer() -> Behaviors::Maximizable&;
  auto progressMaximizer() const -> const Behaviors::Maximizable&;
  auto add_texture_cache(std::shared_ptr<TextureCache> cache) -> void;

 protected:
  UI::Behaviors::Maximizable _maximizer;
  UI::Behaviors::Maximizable _saveLoadMaximizer;
  UI::Behaviors::Maximizable _progressMaximizer;
  std::shared_ptr<TextureCache> _textureCache;
};
}  // namespace Menu
}  // namespace UI