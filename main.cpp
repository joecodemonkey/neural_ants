#include <iostream>
#include <ostream>

#include <raylib.h>
#include <raymath.h>
#include <fmt/core.h>
#include <string>
#include <linux/input-event-codes.h>

#include "population.hpp"
#include "resources.hpp"
#include <iostream>
#include <fstream>

int main(void) {
    Vector2 worldSize = { 1000.0f, 1000.0f };
    Rectangle worldBounds = { 0, 0, worldSize.x, worldSize.y };

    Camera2D camera = { 0 };
    camera.target = (Vector2){ 0.0f, 0.0f };
    camera.offset = (Vector2){ 0.0f, 0.0f };
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;

    Population ant_population(50, worldSize);
    Resources resources(100, worldSize);

    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT);
    InitWindow(worldSize.x, worldSize.y, "raylib - basic window");
    ant_population.set_texture_path("./ant.png");
    // print current directory
    std::cout << "Current directory: " << GetWorkingDirectory() << std::endl;
    std::cout << "Texture path: " << ant_population.get_texture_path() << std::endl;
    // check if the file exists
    std::ifstream file(ant_population.get_texture_path());
    if (!file.good()) {
        std::cout << "File does not exist" << std::endl;
        return 1;
    }

    SetTargetFPS(60);

    float adjust = 1.0f;

    while (!WindowShouldClose()) {
       
        if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) camera.offset = Vector2Add(camera.offset, GetMouseDelta());

        if (IsKeyDown(KEY_LEFT_CONTROL) || IsKeyDown(KEY_RIGHT_CONTROL)) {
            float zoom_adj = 0.0f;

            if (IsKeyDown('=') || IsKeyDown(KEY_KP_ADD)) zoom_adj = 0.01f;
            if (IsKeyDown('-') || IsKeyDown(KEY_KP_SUBTRACT)) zoom_adj = -0.01f;
            if (zoom_adj != 0.0f) {
                float oldZoom = camera.zoom;
                camera.zoom += zoom_adj;
                // Adjust offset to keep the same point under the cursor
                Vector2 mousePos = GetMousePosition();
                Vector2 worldPos = GetScreenToWorld2D(mousePos, camera);
                camera.offset = Vector2Add(camera.offset, Vector2Scale(Vector2Subtract(worldPos, camera.target), (camera.zoom - oldZoom) / oldZoom));
            }

            auto mouse_wheel_move = GetMouseWheelMove();
            if (mouse_wheel_move != 0)
            {
                // Get the world point that is under the mouse
                Vector2 mouseWorldPos = GetScreenToWorld2D(GetMousePosition(), camera);

                // Set the offset to where the mouse is
                camera.offset = GetMousePosition();

                // Set the target to match, so that the camera maps the world space point 
                // under the cursor to the screen space point under the cursor at any zoom
                camera.target = mouseWorldPos;

                // Zoom increment
                // Uses log scaling to provide consistent zoom speed
                float scale = 0.2f*mouse_wheel_move;
                camera.zoom = Clamp(expf(logf(camera.zoom)+scale), 0.125f, 64.0f);
            }
        }

        adjust = 1.0f;
        if (IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT)) adjust = 10.0f;
        if (IsKeyDown('A')) camera.offset.x += 5.0f * adjust;
        if (IsKeyDown('D')) camera.offset.x -= 5.0f * adjust;
        if (IsKeyDown('S')) camera.offset.y -= 5.0f * adjust;
        if (IsKeyDown('W')) camera.offset.y += 5.0f * adjust;

        resources.update();
        resources.feed_ants(ant_population);
        ant_population.update();

        BeginDrawing();
        BeginMode2D(camera);

        ClearBackground(BLACK);

        DrawRectangle(worldBounds.x, worldBounds.y, worldBounds.width, worldBounds.height, WHITE);

        resources.draw();
        ant_population.draw();
        
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
