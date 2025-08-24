#include "raylib.h"

// тестове малювання стрілки
// Функція для малювання трикутної стрілки
void DrawTriangleArrow(Vector2 basePoint, float angleRad, float size) {
    Vector2 points[3];
    points[0] = basePoint;
    points[1].x = basePoint.x + size * cos(angleRad - 30 * DEG2RAD);
    points[1].y = basePoint.y + size * sin(angleRad - 30 * DEG2RAD);
    points[2].x = basePoint.x + size * cos(angleRad + 30 * DEG2RAD);
    points[2].y = basePoint.y + size * sin(angleRad + 30 * DEG2RAD);
    DrawTriangle(points[0], points[1], points[2], BLACK);
}

