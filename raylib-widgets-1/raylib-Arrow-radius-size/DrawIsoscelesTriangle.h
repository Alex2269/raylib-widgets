// DrawIsoscelesTriangle.h

#ifndef DRAWISOSCELESTRIANGLE_H
#define DRAWISOSCELESTRIANGLE_H

#include <math.h>
#include <raylib.h>
#include "DrawIsoscelesTriangle.h"

#define DEG2RAD (3.14159265f / 180.0f)


// Структура для сортування вершин за y
typedef struct {
    Vector2 v[3];
} Triangle;

// Малює рівносторонній трикутник, повернутий на rotationDeg градусів
void DrawTriangleLinesRotated(Vector2 center, float size, float rotationDeg, Color color);
// Порівняння для сортування вершин за y
int CompareY(const void *a, const void *b);
// Лінійна інтерполяція
float m_Lerp(float a, float b, float t);
// Малюємо горизонтальну лінію (scanline) від x1 до x2 на y
void DrawScanline(int y, int x1, int x2, Color color);
// Функція заливки трикутника за вершинами
void FillTriangle(Vector2 v0, Vector2 v1, Vector2 v2, Color color);
// Малюємо рівносторонній трикутник з обертанням і заливкою
void DrawFilledTriangleRotated(Vector2 center, float size, float rotationDeg, Color color);

#endif // DRAWISOSCELESTRIANGLE_H

