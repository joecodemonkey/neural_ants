#include <raylib.h>
#include <math.h>
#include "raymath.h"

// adjust a rectangle's position given rotation so that position is now center
Rectangle RotateRect(Rectangle rectangle, Vector2 position, float rotation) {
  // Draw a line showing the direction the ant is facing  
  const float width = rectangle.width;
  const float height = rectangle.height;

  // Calculate the offset from center
  const float offsetX = width / 2.0F;
  const float offsetY = height / 2.0F;

  // Rotate the offset vector
  const float cos_rot = cos(rotation * DEG2RAD);
  const float sin_rot = sin(rotation * DEG2RAD);
  const float pos_x_adj = offsetX * cos_rot - offsetY * sin_rot;
  const float pos_y_adj = offsetX * sin_rot + offsetY * cos_rot;

  Vector2 new_pos;
  new_pos.x = position.x - pos_x_adj;
  new_pos.y = position.y - pos_y_adj;

  Rectangle new_rect;
  new_rect.x = new_pos.x;
  new_rect.y = new_pos.y;
  new_rect.width = rectangle.width;
  new_rect.height = rectangle.height;
  return new_rect;
}