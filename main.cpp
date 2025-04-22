#include "raylib-cpp.hpp"
#include <fmt/core.h>
#include <string>
#include "population.hpp"
#include "resources.hpp"

int main(void) {
    int screenWidth = 800;
    int screenHeight = 450;
    int fontSize = 20;

    Population ants(5);
    Resources resources(10);

    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT);
    raylib::Window window(screenWidth, screenHeight, "raylib-cpp - basic window");

    SetTargetFPS(60);

    while (!window.ShouldClose()) {

        BeginDrawing();

        window.ClearBackground(RAYWHITE);

        resources.update();
        resources.feed_ants(ants);
        resources.draw();
        ants.update();
        ants.draw();
        EndDrawing();
    }

    return 0;
}
