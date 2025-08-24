// DrawIsoscelesTriangle.h

#ifndef DRAWTRIANGLEARROW_H
#define DRAWTRIANGLEARROW_H

#include "raylib.h"
#include "math.h"

#define DEG2RAD (3.14159265f / 180.0f)

void DrawTriangleArrow(Vector2 basePoint, float rotationDeg, float size, Color color);
void draw_arrow(Vector2 basePoint, float angleRad, float size, Color color);

#endif // DRAWTRIANGLEARROW_H

