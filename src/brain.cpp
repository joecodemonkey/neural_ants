#include "brain.hpp"

#include <algorithm>

#include "ant.hpp"
#include "resources.hpp"
#include "surroundings.hpp"
#include "world.hpp"

auto Brain::get_genome() -> Genome& {
  return _genome;
}

auto Brain::get_genome() const -> const Genome& {
  return _genome;
}

Brain::Brain(Ant& ant, World& world) : _ant(ant), _world(world) {
  _near.set_dimensions(NEAR_TILES_COUNT, NEAR_TILES_COUNT);
  _far.set_dimensions(FAR_TILES_COUNT, FAR_TILES_COUNT);

  const size_t TOTAL_TILES =
      (NEAR_TILES_COUNT * NEAR_TILES_COUNT) + (FAR_TILES_COUNT * FAR_TILES_COUNT);

  _surroundings_encoded.resize(TOTAL_TILES);
  _genome.set_input_count(TOTAL_TILES);
  _genome.set_output_count(2);
  _genome.set_hidden_layer_neuron_count(TOTAL_TILES);
  _genome.set_layer_count(3);
  _genome.randomize();
}

auto Brain::update(float time) -> void {
  _last_update += time;
  if (_last_update < UPDATE_FREQUENCY) {
    return;
  }
  _last_update -= UPDATE_FREQUENCY;

  update_surroundings(_near, NEAR_TILES_SIZE);
  update_surroundings(_far, FAR_TILES_SIZE);

  // Only update the network inputs if the surroundings have changed
  if (_near.changed() || _far.changed()) {
    const auto& near = _near.get_encoded_surroundings();
    std::ranges::copy(near, _surroundings_encoded.begin());

    const auto& far = _far.get_encoded_surroundings();
    std::ranges::copy(far, _surroundings_encoded.begin() + near.size());

    _genome.get_network().set_input(_surroundings_encoded);

    const auto& outputs = _genome.get_network().get_outputs();
    if (outputs.empty()) {
      throw std::runtime_error("Neural network outputs are empty");
    }

    if (outputs.size() != 2) {
      throw std::runtime_error("Neural network outputs are not 2");
    }
    Vector2 velocity;
    velocity.x = outputs[0] * _ant.MAX_VELOCITY;
    velocity.y = outputs[1] * _ant.MAX_VELOCITY;
    // TODO: Renable this
    //  _ant.set_velocity(velocity);
  }
}

auto Brain::update_surroundings(Surroundings& surr, const size_t tile_size) -> void {
  const auto& position = _ant.get_position();
  size_t center = surr.get_height() / 2;  // center is the center x/y tile
  Rectangle rect;
  rect.height = tile_size;
  rect.width = tile_size;
  for (size_t x = 0; x < surr.get_width(); ++x) {
    long long x_rel = x - center;

    for (size_t y = 0; y < surr.get_height(); ++y) {
      // calculate the aabb for this tile
      long long y_rel = y - center;
      rect.x = position.x + x_rel * tile_size;
      rect.y = position.y + y_rel * tile_size;

      if (CheckCollisionRecs(rect, _world.get_bounds())) {
        surr.set_type(x, y, Surroundings::WALL);
      } else if (_world.get_resources().food_in_rect(rect)) {
        surr.set_type(x, y, Surroundings::FOOD);
      } else {
        surr.set_type(x, y, Surroundings::EMPTY);
      }
    }
  }
}
