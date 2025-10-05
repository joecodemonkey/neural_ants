#pragma once
#include <raylib.h>

#include <cstddef>  // For size_t
#include <vector>

#include <neuron.hpp>

/*  Surroundings is a 2D grid of types that are encoded.
    The types are:
    - FOOD
    - EMPTY
    - WALL

    The top-left corner of the grid is (0,0)
    The bottom-right corner of the grid is (width-1, height-1)
    The grid is encoded left to right, top to bottom.
    The encoded vector is a flat vector of values between -1 and 1.
*/

class Surroundings {
 public:

  typedef enum Type { FOOD=0, EMPTY, WALL } Type;
  typedef unsigned char CountType;

  Surroundings() = default;
  Surroundings(const Surroundings& other) = default;
  Surroundings(Surroundings&& other) = delete;
  auto operator=(const Surroundings& other) -> Surroundings& = delete;
  auto operator=(Surroundings&& other) -> Surroundings& = delete;

  auto set_dimensions(size_t width, size_t height) -> void;
  auto set_type(size_t x, size_t y, Type type) -> void;

  auto get_dimensions() const -> Vector2;
  auto get_width() const -> size_t;
  auto get_height() const -> size_t;

  auto get_encoded_surroundings() -> const std::vector<Neuron::Value>&;

  auto changed() const -> bool;

 protected:
  std::vector<std::vector<Type>> _surroundingsType;
  std::vector<Neuron::Value> _surroundingsEncoded;

  bool _changed = false;

  static auto encode_type(Type type) -> float;
  auto update_encoded_surroundings() -> void;
};
