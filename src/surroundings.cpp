#include "surroundings.hpp"

#include <stdexcept>

auto Surroundings::set_dimensions(size_t width, size_t height) -> void {
  // Resize the 2D vector to the specified dimensions
  _surroundingsType.resize(height);
  for (auto& row : _surroundingsType) {
    row.resize(width, EMPTY);  // Initialize all cells as EMPTY
  }

  // Resize the encoded vector to match the total number of cells
  _surroundingsEncoded.resize(width * height);

  // Mark as changed since we've modified the structure
  _changed = true;
}

auto Surroundings::set_type(size_t x, size_t y, Type type) -> void {
  // Check bounds
  if (y < 0 || y >= _surroundingsType.size() || x < 0 || x >= _surroundingsType[0].size()) {
    throw std::out_of_range("Index out of range");
  }

  // Set the type and mark as changed
  _surroundingsType[y][x] = type;

  if (_changed)
    return;

  // Update the encoded vector with this single change
  // to avoid re-encoding the entire vector
  _surroundingsEncoded[y * get_width() + x] = encode_type(type);
}

auto Surroundings::get_dimensions() const -> Vector2 {
  if (_surroundingsType.empty()) {
    return {0, 0};
  }
  return {static_cast<float>(_surroundingsType[0].size()),
          static_cast<float>(_surroundingsType.size())};
}

auto Surroundings::update_encoded_surroundings() -> void {
  // Resize the encoded vector to match the total number of cells
  _surroundingsEncoded.resize(get_width() * get_height());

  // Encode the surroundings
  size_t index = 0;
  for (const auto& row : _surroundingsType) {
    for (const auto& type : row) {
      _surroundingsEncoded[index++] = encode_type(type);
    }
  }

  // Reset the changed flag
  _changed = false;
}

auto Surroundings::get_encoded_surroundings() -> const std::vector<Neuron::Value>& {
  // If the surroundings haven't changed, return the cached encoded vector
  if (!_changed) {
    return _surroundingsEncoded;
  }

  update_encoded_surroundings();

  return _surroundingsEncoded;
}

auto Surroundings::get_width() const -> size_t {
  if (_surroundingsType.empty())
    return 0;

  return _surroundingsType[0].size();
}

auto Surroundings::get_height() const -> size_t {
  return _surroundingsType.size();
}

auto Surroundings::changed() const -> bool {
  return _changed;
}

auto Surroundings::encode_type(Type type) -> float {
  switch (type) {
    case FOOD:
      return 1.0f;
    case EMPTY:
      return 0.0f;
    case WALL:
      return -1.0f;
    default:
      return 0.0f;  // Default case for safety
  }
}