#pragma once

#include <memory>
#include <texture_cache.hpp>
#include <ui/state.hpp>

class Game;

namespace UI {
namespace Menu {
class FitnessDisplay {
 public:
  FitnessDisplay() = default;
  auto draw() -> void;
  auto set_mean(double mean) -> void;
  auto get_mean() -> double;

 protected:
  double _mean;
};
}  // namespace Menu
}  // namespace UI