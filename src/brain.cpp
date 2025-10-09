#include "brain.hpp"

#include <algorithm>
#include <cstdio>

#include "ant.hpp"
#include "neural_network.hpp"
#include "raylib.h"
#include "raylibmathex.h"
#include "resources.hpp"
#include "surroundings.hpp"
#include "world.hpp"

Brain::Brain(World& world, const NeuralNetwork& neuralNetwork)
    : _world(world), _neuralNetwork(neuralNetwork) {
  _surroundings.set_dimensions(TILES_COUNT, TILES_COUNT);
}

auto Brain::operator=(const Brain& other) -> Brain& {
  if (this != &other) {
    _world = other._world;
    _surroundings = other._surroundings;
    _neuralNetwork = other._neuralNetwork;
    _surroundings_encoded = other._surroundings_encoded;
    _last_update = other._last_update;
  }
  return *this;
}

auto Brain::update(float time, Vector2 position) -> Vector2 {
  _last_update += time;
  if (_last_update >= UPDATE_FREQUENCY) {
    _last_update -= UPDATE_FREQUENCY;

    update_surroundings(position);
  }

  // Only update the network inputs if the surroundings have changed
  if (_surroundings.changed()) {
    const auto& far = _surroundings.get_encoded_surroundings();

    // Sanity checks for corrupted sizes
    size_t farSize = far.size();
    const size_t MAX_REASONABLE_SIZE = 1000000;  // 1M elements max
    if (farSize > MAX_REASONABLE_SIZE) {
      throw std::runtime_error("Far surroundings size is suspiciously large: " +
                               std::to_string(farSize) + " (expected ~100)");
    }

    _surroundings_encoded.resize(farSize);
    std::ranges::copy(far, _surroundings_encoded.begin());

    _neuralNetwork.set_input_values(_surroundings_encoded);
  }

  const auto& outputs = _neuralNetwork.get_output_values();
  if (outputs.empty()) {
    throw std::runtime_error("Neural network outputs are empty");
  }

  if (outputs.size() != 2) {
    throw std::runtime_error("Neural network outputs are not 2");
  }
  Vector2 velocity;
  velocity.x = outputs[0] * Ant::MAX_VELOCITY;
  velocity.y = outputs[1] * Ant::MAX_VELOCITY;
  return velocity;
}

auto Brain::update_surroundings(Vector2 position) -> void {
  size_t center = _surroundings.get_height() / 2;  // center is the center x/y tile
  Rectangle rect;
  rect.height = TILES_SIZE;
  rect.width = TILES_SIZE;
  rect.x = position.x;
  rect.y = position.y;
  for (size_t x = 0; x < _surroundings.get_width(); ++x) {
    float x_rel = static_cast<float>(x) - static_cast<float>(center);

    for (size_t y = 0; y < _surroundings.get_height(); ++y) {
      // calculate the aabb for this tile
      float y_rel = static_cast<float>(y) - static_cast<float>(center);
      rect.x = position.x + x_rel * TILES_SIZE;
      rect.y = position.y + y_rel * TILES_SIZE;

      if (!IsRectContained(rect, _world.get().get_bounds())) {
        _surroundings.set_type(x, y, Surroundings::WALL);
      } else if (_world.get().get_resources().food_in_rect(rect)) {
        _surroundings.set_type(x, y, Surroundings::FOOD);
      } else {
        _surroundings.set_type(x, y, Surroundings::EMPTY);
      }
    }
  }
}
