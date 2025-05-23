#include "resources.hpp"

#include <algorithm>

void Resources::update() {
  if (_food.size() > _intended_food_count) {
    // stupidly inefficient, but it works and we don't care about performance right now
    std::remove_if(_food.begin(), _food.end(), [](const Food& food) { return food.is_eaten(); });
  }

  // Add food to the resources
  while (_food.size() < _intended_food_count) {
    _food.push_back(new_food());
  }

  // replace the food that has been eaten,
  // we should probably do this in a more efficient way
  for (Food& food : _food) {
    if (food.is_eaten()) {
      food = new_food();
    }
  }
}

void Resources::draw() {
  for (auto& food : _food) {
    food.draw();
  }
}

Food Resources::new_food() {
  // The new food will be within 20% of the bounds of the screen
  auto x = GetRandomValue(_worldSize.x * 0.2, _worldSize.x * 0.8);
  auto y = GetRandomValue(_worldSize.y * 0.2, _worldSize.y * 0.8);
  return Food(Vector2(x, y));
}

void Resources::feed_ants(Population& population) {
  for (Food& food : _food) {
    auto ants = population.find_touching(food.get_position(), food.get_size());
    for (auto& ant : ants) {
      food.eat(ant.get());
      break;  // Only one ant can eat the food at a time
    }
  }
}

void Resources::set_world_size(const Vector2& worldSize) {
  _worldSize = worldSize;
}

[[nodiscard]] auto Resources::get_world_size() const -> const Vector2& {
  return _worldSize;
}