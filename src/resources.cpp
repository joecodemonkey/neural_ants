#include "resources.hpp"

#include "food.hpp"
#include "raylib.h"
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

    auto ants = population.get_collisions(food.get_position(), food.get_radius());

    if (ants.size() == 0)
      continue;

    for (auto& ant : ants) {
      food.eat(ant.get());
      break;  // Only one ant can eat the food at a time
    }
  }
}

auto Resources::get_food_count() const -> int {
  return _food_count;
}

auto Resources::set_food_count(int size) -> void {
  _food_count = size;
}

auto Resources::food_in_rect(const Rectangle& rect) const -> bool {
  for (const Food& food : _food) {
    if (CheckCollisionCircleRec(food.get_position(), food.get_radius(), rect)) {
      return true;
    }
  }
  return false;
}