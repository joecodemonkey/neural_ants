#include "resources.hpp"

#include "food.hpp"
#include "raylib.h"
#include "world.hpp"

Resources::Resources(World& world) : _world(world) {
  _food_count = DEFAULT_COUNT;
}

Resources::Resources(const nlohmann::json& json, World& world) : _world(world) {
  _food_count = json.at("food_count").get<size_t>();

  auto& textureCache = *_world.get_texture_cache();
  for (const auto& food_json : json.at("food")) {
    _food.push_back(Food(food_json, textureCache));
  }
}

Resources::Resources(const Resources& other) : _world(other._world) {
  _food_count = other._food_count;
  _food = other._food;
}

Resources& Resources::operator=(const Resources& other) {
  if (this != &other) {
    _food_count = other._food_count;
    _food = other._food;
  }
  return *this;
}

bool Resources::operator==(const Resources& other) const {
  return _food_count == other._food_count && _food == other._food;
}

auto Resources::update(float time) -> void {
  // Add food to the resources
  auto& textureCache = *_world.get_texture_cache();
  while (_food.size() < _food_count)
    _food.push_back(Food(_world.spawn_position({Food::TEXTURE_WIDTH, Food::TEXTURE_HEIGHT}), textureCache));

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
      food.reset(_world.spawn_position({Food::TEXTURE_WIDTH, Food::TEXTURE_HEIGHT}));
      // Randomize sprite when food respawns
      auto& textureCache = *_world.get_texture_cache();
      food.set_texture_index(textureCache.get_random_texture_index("food_"));
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

auto Resources::to_json() const -> nlohmann::json {
  nlohmann::json json;
  json["food_count"] = _food_count;

  nlohmann::json food_array = nlohmann::json::array();
  for (const auto& food : _food) {
    food_array.push_back(food.to_json());
  }
  json["food"] = food_array;

  return json;
}