#include "brain.hpp"

Brain::Brain() : _direction(0.0f), _speed(0.0f), _far_tile_size(10), _far_tile_window_size(5) {
    _near_eye.set_tile_size(1);
    _near_eye.set_tile_window_size(_far_tile_size);
    _far_eye.set_tile_size(_far_tile_size);
    _far_eye.set_tile_window_size(_far_tile_window_size);
}

void Brain::update(const Population &population, const Resources &resources, const Vecotr2 &position) {
     
     
    // Get encoded surroundings for neural network input
    const auto& encoded_surroundings = _surroundings.get_encoded_surroundings();
    
    // Update neural network with surroundings
    _genome.get_neural_network().set_input(encoded_surroundings);
    
    // Get outputs from neural network
    const auto& outputs = _genome.get_neural_network().get_outputs();
    
    // Update direction and speed based on neural network outputs
    if (outputs.size() >= 2) {
        _direction = outputs[0] * 360.0f;  // Convert to degrees
        _speed = outputs[1] * 5.0f;        // Scale speed appropriately
    }
}

void Brain::set_population(Population *population) {
    this->population = population;
}

Population *Brain::get_population() {
    return population;
}

void Brain::set_resources(Resources *resources) {
    this->resources = resources;
}

Resources *Brain::get_resources() {
    return resources;
}

float Brain::get_speed() const {
    return _speed;
}

float Brain::get_direction() const {
    return _direction;
}

void Brain::set_bounds(Rectangle &bounds) {
    _bounds = bounds;
}

Rectangle &Brain::get_bounds() {
    return _bounds;
}

const Rectangle &Brain::get_bounds() const {
    return _bounds;
}

Genome &Brain::get_genome() {
    return _genome;
}

const Genome &Brain::get_genome() const {
    return _genome;
}

void Brain::set_genome(const Genome &genome) {
    _genome = genome;
}

Vector2 Brain::get_position() const {
    return {_bounds.x, _bounds.y};
}

void Brain::set_position(Vector2 position) {
    _bounds.x = position.x;
    _bounds.y = position.y;
}

void Brain::set_tile_window_size(size_t size) {
    if (size %2 != 1) {
        throw std::invalid_argument("Tile window size must be an odd number");
    }
    _tile_window_size = size;
}

size_t Brain::get_tile_window_size() const {
    return _tile_window_size;
}

void Brain::set_tile_size(size_t size) {
    _tile_size = size;
}

size_t Brain::get_tile_size() const {
    return _tile_size;
}

void Brain::set_tile_window_size(size_t size) {
    _surroundings.set_dimensions(size, size);
}

size_t Brain::get_tile_window_size() const {
    Vector2 dims = _surroundings.get_dimensions();
    return static_cast<size_t>(dims.x);  // Assuming square window
}

void Brain::set_food() {
    if (!resources) return;
    
    Vector2 pos = get_position();
    int center_x = static_cast<int>(pos.x / _tile_size);
    int center_y = static_cast<int>(pos.y / _tile_size);
    
    // Check surrounding tiles for food
    for (int y = -_tile_size/2; y <= _tile_size/2; ++y) {
        for (int x = -_tile_size/2; x <= _tile_size/2; ++x) {
            if (resources->has_food(center_x + x, center_y + y)) {
                _surroundings.set_type(x + _tile_size/2, y + _tile_size/2, Surroundings::FOOD);
            }
        }
    }
}

void Brain::set_ants() {
    if (!population) return;
    
    Vector2 pos = get_position();
    int center_x = static_cast<int>(pos.x / _tile_size);
    int center_y = static_cast<int>(pos.y / _tile_size);
    
    // Check surrounding tiles for other ants
    for (int y = -_tile_size/2; y <= _tile_size/2; ++y) {
        for (int x = -_tile_size/2; x <= _tile_size/2; ++x) {
            if (population->has_ant_at(center_x + x, center_y + y)) {
                _surroundings.set_type(x + _tile_size/2, y + _tile_size/2, Surroundings::ANT);
            }
        }
    }
}

void Brain::set_empty() {
    // Set all tiles to empty by default
    for (int y = 0; y < _tile_size; ++y) {
        for (int x = 0; x < _tile_size; ++x) {
            _surroundings.set_type(x, y, Surroundings::EMPTY);
        }
    }
}


Rectangle Brain::tile_to_aabb(int tile_x, int tile_y) const {
    // Convert tile coordinates to world space relative to ant's position
    // tile_x and tile_y are relative to the ant's position
    
    Rectangle world_aabb;
    
    // Get the ant's position in tile coordinates
    Vector2 ant_pos = get_position();

    // Ant is in the center tile given the tile window size and tile size
    int ant_tile_x = _tile_window_size / 2;
    int ant_tile_y = _tile_window_size / 2;

    int tile_delta_x = tile_x - ant_tile_x;
    int tile_delta_y = tile_y - ant_tile_y;

    float world_x = ant_pos.x + tile_delta_x * _tile_size;
    float world_y = ant_pos.y + tile_delta_y * _tile_size;

    world_aabb.x = world_x;
    world_aabb.y = world_y;
    world_aabb.width = _tile_size;
    world_aabb.height = _tile_size;

    return world_aabb;
}
