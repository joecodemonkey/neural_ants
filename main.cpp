#include "raylib-cpp.hpp"
#include <fmt/core.h>
#include <string>
#include "population.hpp"

int main(void) {
    int screenWidth = 800;
    int screenHeight = 450;
    int fontSize = 20;

    Population ants(5);

    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT);
    raylib::Window window(screenWidth, screenHeight, "raylib-cpp - basic window");

    std::string text = fmt::format("Window Size: {} x {}", screenWidth, screenHeight);
    auto textSize = MeasureTextEx(GetFontDefault(), text.c_str(), fontSize, 1.0f);
    int textX = (screenWidth - textSize.x) / 2;
    int textY = (screenHeight - textSize.y) / 2;

    const float desiredTextWidthRatio = static_cast<float>(textSize.x) / screenWidth;

    SetTargetFPS(60);

    while (!window.ShouldClose()) {
        if (window.IsResized()) {
            screenWidth = window.GetWidth();
            screenHeight = window.GetHeight();
            textX = (screenWidth - textSize.x) / 2;
            textY = (screenHeight - textSize.y) / 2;
            text = fmt::format("Window Size: {} x {}", screenWidth, screenHeight);
        }

        BeginDrawing();

        window.ClearBackground(RAYWHITE);

        ants.update();
        ants.draw();

        EndDrawing();
    }

    return 0;
}
