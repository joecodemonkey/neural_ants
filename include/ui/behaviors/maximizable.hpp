#pragma once
namespace UI {
namespace Behaviors {

class Maximizable {
 public:
  Maximizable();
  auto maximized() const -> bool;
  auto maximize() -> void;
  auto minimize() -> void;

 protected:
  bool _maximized;
};
}  // namespace Behaviors
}  // namespace UI