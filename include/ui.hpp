#pragma once

class UI {
 public:
  UI() = default;
  auto update(float deltaTime) -> void;

 protected:
  bool _setup = false;
  auto setup() -> void;

  auto update_right_panel(float deltaTime) -> void;

  int _screenWidth;
  int _screenHeight;
};