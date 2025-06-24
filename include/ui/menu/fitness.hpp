#include <fitness_data.hpp>
#include <memory>
#include <texture_cache.hpp>
#include <ui/state.hpp>
namespace UI {
namespace Menu {
class Fitness {
 public:
  Fitness(UI::State& state);
  auto draw() -> void;
  auto add_texture_cache(std::shared_ptr<TextureCache> cache) -> void;
  auto add_fitness_data(std::shared_ptr<FitnessData> fitnessData) -> void;

 protected:
  UI::State& _state;
  std::shared_ptr<TextureCache> _textureCache;
};
}  // namespace Menu
}  // namespace UI