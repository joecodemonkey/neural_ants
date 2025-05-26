#ifndef RAYLIBDRAWEX_H
#define RAYLIBDRAWEX_H

#include <raylib.h>

#include "math.h"

#ifdef __cplusplus
extern "C" {
#endif

void DrawRectangleLinesExRot(
    Rectangle rectangle, Vector2 position, float rotation, float scale, Color color);

#ifdef __cplusplus
}
#endif

#endif  // RAYLIBDRAWEX_H