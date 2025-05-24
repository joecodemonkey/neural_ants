#include "resources.hpp"

#include <algorithm>

#include "world.hpp"

Resources::Resources(World& world) : _world(world), _food_count(DEFAULT_COUNT) {}

auto Resources::update(float time) -> void {
  // TODO: Optimize
  std::remove_if(_food.begin(), _food.end(), [](const Food& food) { return food.is_eaten(); });

  // Add food to the resources
  while (_food.size() < _food_count)
    _food.push_back(new_food());

  feed_ants(_world.get_population());
}

auto Resources::draw() const -> void {
  for (const auto& food : _food) {
    food.draw();
  }
}

auto Resources::new_food() -> Food {
  const auto width = _world.get_bounds().width;
  const auto height = _world.get_bounds().height;

  auto x = GetRandomValue(width * BOUNDS_PADDING, width * (1.0 - BOUNDS_PADDING));
  auto y = GetRandomValue(height * BOUNDS_PADDING, height * (1.0 - BOUNDS_PADDING));
  return Food(Vector2(x, y));
}

auto Resources::feed_ants(Population& population) -> void {
  for (Food& food : _food) {
    if (food.is_eaten())
      continue;

    auto ants = population.find_touching(food.get_position(), food.get_size());

    if (ants.size() == 0)
      continue;

    for (auto& ant : ants) {
      food.eat(ant.get());
      break;  // Only one ant can eat the food at a time
    }
  }
}

auto Resources::food_in_rect(const Rectangle rect) const -> bool {
  Rectangle food_rect;
  for (const Food& food : _food) {
    if (CheckCollisionRecs(rect, food.get_bounds())) {
      return true;
    }
  }
  return false;
}

auto Resources::get_food_count() const -> int {
  return _food_count;
}

auto Resources::set_food_count(int size) -> void {
  _food_count = size;
}