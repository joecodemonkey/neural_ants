#include <raylib.h>
#include "math.h"

void DrawRectangleLinesExRot(Rectangle rectangle, Vector2 position, float rotation, float scale, Color color) {

  // Convert rotation to radians
  float rad = rotation * DEG2RAD;
  float cosAngle = cosf(rad);
  float sinAngle = sinf(rad);

  // Calculate half dimensions
  float halfWidth = rectangle.width * 0.5f * scale;
  float halfHeight = rectangle.height * 0.5f * scale;

  // Calculate the four corners relative to center
  Vector2 corners[4] = {
      {-halfWidth, -halfHeight},  // Top-left
      {halfWidth, -halfHeight},   // Top-right
      {halfWidth, halfHeight},    // Bottom-right
      {-halfWidth, halfHeight}    // Bottom-left
  };

  // Rotate and translate each corner
  for (int i = 0; i < 4; i++) {
    float rotatedX = corners[i].x * cosAngle - corners[i].y * sinAngle;
    float rotatedY = corners[i].x * sinAngle + corners[i].y * cosAngle;
    corners[i].x = position.x + rotatedX;
    corners[i].y = position.y + rotatedY;
  }

  // Draw lines between consecutive corners
  DrawLineV(corners[0], corners[1], color);
  DrawLineV(corners[1], corners[2], color);
  DrawLineV(corners[2], corners[3], color);
  DrawLineV(corners[3], corners[0], color);
}