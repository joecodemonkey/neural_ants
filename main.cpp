#include <fmt/core.h>
#include <raylib.h>
#include <raymath.h>

#include <fstream>
#include <iostream>
#include <ostream>
#include <string>

#include "population.hpp"
#include "resources.hpp"
#include "world.hpp"

int main(void) {
  Camera2D camera = {0};
  camera.target = Vector2{0.0f, 0.0f};
  camera.offset = Vector2{0.0f, 0.0f};
  camera.rotation = 0.0f;
  camera.zoom = 1.0f;

  World world;

  world.get_population().set_size(50);
  world.get_population().set_texture_path("./ant.png");

  world.get_resources().set_food_count(100);
  world.get_resources().set_world_size({world.get_bounds().height, world.get_bounds().width});

  SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT);
  InitWindow(world.get_bounds().width, world.get_bounds().height, "Neural Ants");
  SetTargetFPS(60);

  float adjust = 1.0f;

  while (!WindowShouldClose()) {
    if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT))
      camera.offset = Vector2Add(camera.offset, GetMouseDelta());

    if (IsKeyDown(KEY_LEFT_CONTROL) || IsKeyDown(KEY_RIGHT_CONTROL)) {
      float zoom_adj = 0.0f;

      if (IsKeyDown('=') || IsKeyDown(KEY_KP_ADD))
        zoom_adj = 0.01f;
      if (IsKeyDown('-') || IsKeyDown(KEY_KP_SUBTRACT))
        zoom_adj = -0.01f;
      if (zoom_adj != 0.0f) {
        float oldZoom = camera.zoom;
        camera.zoom += zoom_adj;
        // Adjust offset to keep the same point under the cursor
        Vector2 mousePos = GetMousePosition();
        Vector2 worldPos = GetScreenToWorld2D(mousePos, camera);
        camera.offset = Vector2Add(camera.offset,
                                   Vector2Scale(Vector2Subtract(worldPos, camera.target),
                                                (camera.zoom - oldZoom) / oldZoom));
      }

      auto mouse_wheel_move = GetMouseWheelMove();
      if (mouse_wheel_move != 0) {
        // Get the world point that is under the mouse
        Vector2 mouseWorldPos = GetScreenToWorld2D(GetMousePosition(), camera);

        // Set the offset to where the mouse is
        camera.offset = GetMousePosition();

        // Set the target to match, so that the camera maps the world space point
        // under the cursor to the screen space point under the cursor at any zoom
        camera.target = mouseWorldPos;

        // Zoom increment
        // Uses log scaling to provide consistent zoom speed
        float scale = 0.2f * mouse_wheel_move;
        camera.zoom = Clamp(expf(logf(camera.zoom) + scale), 0.125f, 64.0f);
      }
    }

    adjust = 1.0f;
    if (IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT))
      adjust = 10.0f;
    if (IsKeyDown('A'))
      camera.offset.x += 5.0f * adjust;
    if (IsKeyDown('D'))
      camera.offset.x -= 5.0f * adjust;
    if (IsKeyDown('S'))
      camera.offset.y -= 5.0f * adjust;
    if (IsKeyDown('W'))
      camera.offset.y += 5.0f * adjust;

    BeginDrawing();
    BeginMode2D(camera);
    world.update(GetFrameTime());
    ClearBackground(BLACK);

    world.draw();

    EndDrawing();
  }

  CloseWindow();
  return 0;
}
