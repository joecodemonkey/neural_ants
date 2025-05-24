#include "resources.hpp"

#include "food.hpp"
#include "world.hpp"

Resources::Resources(World& world) : _world(world) {
  _food_count = DEFAULT_COUNT;
}

auto Resources::update(float time) -> void {
  // Add food to the resources
  while (_food.size() < _food_count)
    _food.push_back(Food(_world.spawn_position({Food::DEFAULT_SIZE, Food::DEFAULT_SIZE})));

  feed_ants(_world.get_population());
}

auto Resources::draw() const -> void {
  for (const auto& food : _food) {
    food.draw();
  }
}

auto Resources::feed_ants(Population& population) -> void {
  for (Food& food : _food) {
    if (food.is_eaten()) {
      food.reset(_world.spawn_position({Food::DEFAULT_SIZE, Food::DEFAULT_SIZE}));
    }

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
    if (food.is_eaten()) {
      continue;
    }
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