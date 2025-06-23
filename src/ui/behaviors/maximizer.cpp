#include "ui/behaviors/maximizable.hpp"

namespace UI {
namespace Behaviors {

Maximizable::Maximizable() : _maximized(false) {}

auto Maximizable::maximized() const -> bool {
  return _maximized;
}

auto Maximizable::maximize() -> void {
  _maximized = true;
}

auto Maximizable::minimize() -> void {
  _maximized = false;
}

}  // namespace Behaviors
}  // namespace UI
