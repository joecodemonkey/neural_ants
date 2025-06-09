#ifndef RAYLIBMATHEX_H
#define RAYLIBMATHEX_H

#include <raylib.h>

#ifdef __cplusplus
extern "C" {
#endif

// adjust a rectangle's position given rotation so that position is now its center
Rectangle RotateRect(Rectangle rectangle, Vector2 position, float rotation);

// check if a rect (inner) is within a rect (outer)
bool IsRectContained(Rectangle inner, Rectangle outer);

#ifdef __cplusplus
}
#endif

#endif  // RAYLIBMATHEX_H