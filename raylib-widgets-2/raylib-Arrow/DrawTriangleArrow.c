#include "DrawTriangleArrow.h"

// #define DEG2RAD (PI/180.0f)

// double deg2rad(double deg) {
//     return deg * M_PI / 180.0;
// }

// тестове малювання стрілки
// Функція для малювання трикутної стрілки
void DrawTriangleArrow(Vector2 basePoint, float rotationDeg, float size, Color color) {
    float angleRad = rotationDeg * DEG2RAD;
    Vector2 points[3];
    points[0] = basePoint;
    points[1].x = basePoint.x + size * cos(angleRad - 30 * DEG2RAD);
    points[1].y = basePoint.y + size * sin(angleRad - 30 * DEG2RAD);
    points[2].x = basePoint.x + size * cos(angleRad + 30 * DEG2RAD);
    points[2].y = basePoint.y + size * sin(angleRad + 30 * DEG2RAD);
    DrawTriangle(points[0], points[2], points[1], color);
}

