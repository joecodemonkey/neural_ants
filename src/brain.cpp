#include "brain.hpp"

#include <algorithm>
#include <cstdio>
#include <iostream>

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
    _surroundings_encoded.resize(_near.get_encoded_surroundings().size() +
                                 _far.get_encoded_surroundings().size());
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
  for (float x = 0; x < surr.get_width(); ++x) {
    float x_rel = x - center;

    for (float y = 0; y < surr.get_height(); ++y) {
      // calculate the aabb for this tile
      float y_rel = y - center;
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