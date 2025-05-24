#include "resources.hpp"

#include <algorithm>

#include "food.hpp"
#include "world.hpp"

Resources::Resources(World& world) : _world(world) {
  _intended_food_count = DEFAULT_COUNT;
}

void Resources::update(float time) {
  // TODO: Optimize
  std::remove_if(_food.begin(), _food.end(), [](const Food& food) { return food.is_eaten(); });

  // Add food to the resources
  while (_food.size() < _intended_food_count)
    _food.push_back(new_food());

  feed_ants(_world.get_population());
}

void Resources::draw() const {
  for (const auto& food : _food) {
    food.draw();
  }
}

Food Resources::new_food() {
  const auto width = _world.get_bounds().width;
  const auto height = _world.get_bounds().height;

  auto x = GetRandomValue(width * BOUNDS_PADDING, width * (1.0 - BOUNDS_PADDING));
  auto y = GetRandomValue(height * BOUNDS_PADDING, height * (1.0 - BOUNDS_PADDING));
  return Food(Vector2(x, y));
}

void Resources::feed_ants(Population& population) {
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

bool Resources::food_in_rect(const Rectangle rect) const {
  Rectangle food_rect;
  for (const Food& food : _food) {
    if (CheckCollisionRecs(rect, food.get_bounds())) {
      return true;
    }
  }
  return false;
}