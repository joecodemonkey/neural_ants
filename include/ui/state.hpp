#pragma once
#include <imgui.h>
#include <raylib.h>

#include <unordered_set>

namespace UI {

class State {
 public:
  typedef enum Component { SETTINGS, PROGRESS, SAVELOAD } Component;

  State() = default;
  auto maximize(Component component) -> void;
  auto minimize(Component component) -> void;
  auto is_maximized(Component) -> bool;

 protected:
  std::unordered_set<Component> _maximzedComponents;
};
}  // namespace UI