#include <fmt/format.h>
#include <imgui.h>
#include <raylib.h>

#include <ui/menu/fitness_display.hpp>

#include "game.hpp"
#include "ui/state.hpp"

auto UI::Menu::FitnessDisplay::draw() -> void {
  // Get screen dimensions for scaling
  const int screenWidth = GetScreenWidth();
  const int screenHeight = GetScreenHeight();

  // Calculate scaled font size based on screen size
  // Base font size is 72, scale down for smaller screens
  const int baseFontSize = 32;
  const int minFontSize = 16;          // Minimum readable font size
  const int minScreenDimension = 400;  // Threshold for scaling

  int fontSize = baseFontSize;
  if (screenWidth < minScreenDimension || screenHeight < minScreenDimension) {
    // Scale down proportionally, but not below minimum
    float scale = static_cast<float>(std::min(screenWidth, screenHeight)) / minScreenDimension;
    fontSize = std::max(minFontSize, static_cast<int>(baseFontSize * scale));
  }

  const Color textColor = RED;
  const Vector2 position = {20, 20};  // Upper left corner with some margin

  std::string fitness = fmt::format("Fitness: {:.2f}", _mean);

  DrawText(fitness.c_str(),
           static_cast<int>(position.x),
           static_cast<int>(position.y),
           fontSize,
           textColor);
}

auto UI::Menu::FitnessDisplay::set_mean(double mean) -> void {
  _mean = mean;
}
auto UI::Menu::FitnessDisplay::get_mean() -> double {
  return _mean;
}
