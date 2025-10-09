#pragma once

#include <unordered_set>

namespace UI {

class State {
 public:
  typedef enum Component { SETTINGS, MEAN_FITNESS, SAVELOAD } Component;

  State() = default;
  auto maximize(Component component) -> void;
  auto minimize(Component component) -> void;
  auto is_maximized(Component) -> bool;
  auto toggle(Component component) -> void;

 protected:
  std::unordered_set<Component> _maximzedComponents;
};
}