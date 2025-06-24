#include "ui/state.hpp"

namespace UI {

auto State::maximize(Component component) -> void {
  _maximzedComponents.insert(component);
}

auto State::minimize(Component component) -> void {
  _maximzedComponents.erase(component);
}

auto State::is_maximized(Component component) -> bool {
  return _maximzedComponents.find(component) != _maximzedComponents.end();
}

}  // namespace UI