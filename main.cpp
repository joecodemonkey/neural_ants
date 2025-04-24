#include <iostream>
#include <ostream>

#include "raylib-cpp.hpp"
#include <fmt/core.h>
#include <string>
#include <linux/input-event-codes.h>

#include "population.hpp"
#include "resources.hpp"
#include <iostream>

int main(void) {
    raylib::Vector2 worldSize = { 1000.0f, 1000.0f };
    raylib::Rectangle worldBounds = { 0, 0, worldSize.x, worldSize.y };

    Camera2D camera = { 0 };
    camera.target = (Vector2){ 0.0f, 0.0f };
    camera.offset = (Vector2){ 0.0f, 0.0f };
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;

    Population ants(50, worldSize);
    Resources resources(100, worldSize);

    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT);
    raylib::Window window(worldSize.x, worldSize.y, "raylib-cpp - basic window");

    SetTargetFPS(60);

    float adjust = 1.0f;
    while (!window.ShouldClose()) {

        if (IsKeyDown(KEY_LEFT_CONTROL) || IsKeyDown(KEY_RIGHT_CONTROL)) {
            if (IsKeyDown('=') || IsKeyDown(KEY_KP_ADD)) camera.zoom += 0.01f;
            if (IsKeyDown('-') || IsKeyDown(KEY_KP_SUBTRACT)) camera.zoom -= 0.01f;
        }

        adjust = 1.0f;
        if (IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT)) adjust = 10.0f;
        if (IsKeyDown('A')) camera.offset.x += 5.0f * adjust;
        if (IsKeyDown('D')) camera.offset.x -= 5.0f * adjust;
        if (IsKeyDown('S')) camera.offset.y -= 5.0f * adjust;
        if (IsKeyDown('W')) camera.offset.y += 5.0f * adjust;

        resources.update();
        resources.feed_ants(ants);
        ants.update();

        BeginDrawing();
        BeginMode2D(camera);

        window.ClearBackground(BLACK);

        DrawRectangle(worldBounds.x, worldBounds.y, worldBounds.width, worldBounds.height, raylib::Color::White());

        resources.draw();
        ants.draw();
        EndDrawing();
    }

    return 0;
}
