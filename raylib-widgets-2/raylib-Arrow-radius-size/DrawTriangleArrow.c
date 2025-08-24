#include "raylib.h"
#include "math.h"

double deg2rad(double deg) {
    return deg * M_PI / 180.0;
}

// тестове малювання стрілки
// Функція для малювання трикутної стрілки
void DrawTriangleArrow(Vector2 basePoint, float angleRad, float size, Color color) {
    Vector2 points[3];
    points[0] = basePoint;
    points[1].x = basePoint.x + size * cos(angleRad + deg2rad(30));
    points[1].y = basePoint.y + size * sin(angleRad + deg2rad(30));
    points[2].x = basePoint.x + size * cos(angleRad + deg2rad(-30));
    points[2].y = basePoint.y + size * sin(angleRad + deg2rad(-30));
    DrawTriangle(points[0], points[1], points[2], color);
}

// тестове малювання стрілки
// Функція для малювання трикутної стрілки
void draw_arrow(Vector2 basePoint, float angleRad, float size, Color color) {
    Vector2 points[3];
    points[0] = basePoint;
    points[1].x = basePoint.x + size * cos(angleRad + 30 * DEG2RAD);
    points[1].y = basePoint.y + size * sin(angleRad + 30 * DEG2RAD);
    points[2].x = basePoint.x + size * cos(angleRad - 30 * DEG2RAD);
    points[2].y = basePoint.y + size * sin(angleRad - 30 * DEG2RAD);
    DrawTriangle(points[0], points[1], points[2], color);
}

