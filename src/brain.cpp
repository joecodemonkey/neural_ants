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
  _near.set_dimensions(NEAR_TILES_COUNT, NEAR_TILES_COUNT);
  _far.set_dimensions(FAR_TILES_COUNT, FAR_TILES_COUNT);
}

auto Brain::update(float time, Vector2 position) -> Vector2 {
  _last_update += time;
  if (_last_update >= UPDATE_FREQUENCY) {
    _last_update -= UPDATE_FREQUENCY;

    update_surroundings(_near, NEAR_TILES_SIZE, position);
    update_surroundings(_far, FAR_TILES_SIZE, position);
  }

  // Only update the network inputs if the surroundings have changed
  if (_near.changed() || _far.changed()) {
    const auto& near = _near.get_encoded_surroundings();
    const auto& far = _far.get_encoded_surroundings();

    // Sanity checks for corrupted sizes
    size_t nearSize = near.size();
    size_t farSize = far.size();
    const size_t MAX_REASONABLE_SIZE = 1000000;  // 1M elements max
    if (nearSize > MAX_REASONABLE_SIZE) {
      throw std::runtime_error("Near surroundings size is suspiciously large: " +
                               std::to_string(nearSize) + " (expected ~25)");
    }
    if (farSize > MAX_REASONABLE_SIZE) {
      throw std::runtime_error("Far surroundings size is suspiciously large: " +
                               std::to_string(farSize) + " (expected ~25)");
    }

    // Check for overflow in size addition
    if (nearSize > SIZE_MAX - farSize) {
      throw std::overflow_error("Combined surroundings size would overflow");
    }

    size_t totalSize = nearSize + farSize;
    _surroundings_encoded.resize(totalSize);
    std::ranges::copy(near, _surroundings_encoded.begin());
    std::ranges::copy(far, _surroundings_encoded.begin() + near.size());

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

auto Brain::update_surroundings(Surroundings& surr, const size_t tile_size, Vector2 position)
    -> void {
  size_t center = surr.get_height() / 2;  // center is the center x/y tile
  Rectangle rect;
  rect.height = tile_size;
  rect.width = tile_size;
  rect.x = position.x;
  rect.y = position.y;
  for (size_t x = 0; x < surr.get_width(); ++x) {
    float x_rel = static_cast<float>(x) - static_cast<float>(center);

    for (size_t y = 0; y < surr.get_height(); ++y) {
      // calculate the aabb for this tile
      float y_rel = static_cast<float>(y) - static_cast<float>(center);
      rect.x = position.x + x_rel * tile_size;
      rect.y = position.y + y_rel * tile_size;

      if (!IsRectContained(rect, _world.get_bounds())) {
        surr.set_type(x, y, Surroundings::WALL);
      } else if (_world.get_resources().food_in_rect(rect)) {
        surr.set_type(x, y, Surroundings::FOOD);
      } else {
        surr.set_type(x, y, Surroundings::EMPTY);
      }
    }
  }
}
