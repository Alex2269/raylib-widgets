#include "DrawIsoscelesTriangle.h"
#include <math.h>
#include <stdlib.h>

#define DEG2RAD (3.14159265f / 180.0f)

// Порівняння для сортування вершин за y
int CompareY(const void *a, const void *b) {
    Vector2 *va = (Vector2 *)a;
    Vector2 *vb = (Vector2 *)b;
    if (va->y < vb->y) return -1;
    else if (va->y > vb->y) return 1;
    else return 0;
}

// Лінійна інтерполяція
float m_Lerp(float a, float b, float t) {
    return a + t * (b - a);
}

// Малюємо горизонтальну лінію (scanline) від x1 до x2 на y
void DrawScanline(int y, int x1, int x2, Color color) {
    if (x1 > x2) {
        int tmp = x1; x1 = x2; x2 = tmp;
    }
    for (int x = x1; x <= x2; x++) {
        DrawPixel(x, y, color);
    }
}

// Функція заливки трикутника за вершинами
void FillTriangle(Vector2 v0, Vector2 v1, Vector2 v2, Color color) {
    // Сортуємо вершини за y (v0.y <= v1.y <= v2.y)
    Vector2 verts[3] = {v0, v1, v2};
    qsort(verts, 3, sizeof(Vector2), CompareY);
    v0 = verts[0];
    v1 = verts[1];
    v2 = verts[2];

    // Визначаємо довжини відрізків по y
    float total_height = v2.y - v0.y;
    if (total_height == 0) return; // уникаємо ділення на 0

    // Верхня частина трикутника
    float segment_height = v1.y - v0.y;
    if (segment_height > 0) {
        for (int y = (int)ceilf(v0.y); y <= (int)floorf(v1.y); y++) {
            float alpha = (y - v0.y) / total_height;
            float beta = (y - v0.y) / segment_height;
            float Ax = m_Lerp(v0.x, v2.x, alpha);
            float Bx = m_Lerp(v0.x, v1.x, beta);
            DrawScanline(y, (int)(Ax + 0.5f), (int)(Bx + 0.5f), color);
        }
    }

    // Нижня частина трикутника
    segment_height = v2.y - v1.y;
    if (segment_height > 0) {
        for (int y = (int)ceilf(v1.y); y <= (int)floorf(v2.y); y++) {
            float alpha = (y - v0.y) / total_height;
            float beta = (y - v1.y) / segment_height;
            float Ax = m_Lerp(v0.x, v2.x, alpha);
            float Bx = m_Lerp(v1.x, v2.x, beta);
            DrawScanline(y, (int)(Ax + 0.5f), (int)(Bx + 0.5f), color);
        }
    }
}

// Малює рівносторонній трикутник, повернутий на rotationDeg градусів (лінії)
void DrawTriangleLinesRotated(Vector2 center, float size, float rotationDeg, Color color) {
    float angleRad = rotationDeg * DEG2RAD;
    float height = (sqrtf(3.0f) / 2.0f) * size;  // Висота рівностороннього трикутника

    Vector2 points[3];

    // Вершини трикутника відносно центра (0,0)
    points[0].x = 0;
    points[0].y = -2.0f / 3.0f * height;       // Верхня вершина

    points[1].x = -size / 2.0f;
    points[1].y = 1.0f / 3.0f * height;

    points[2].x = size / 2.0f;
    points[2].y = 1.0f / 3.0f * height;

    // Поворот і зсув відносно центра
    for (int i = 0; i < 3; i++) {
        float xRot = points[i].x * cosf(angleRad) - points[i].y * sinf(angleRad);
        float yRot = points[i].x * sinf(angleRad) + points[i].y * cosf(angleRad);
        points[i].x = center.x + xRot;
        points[i].y = center.y + yRot;
    }

    // Малюємо лінії між вершинами
    DrawLineV(points[0], points[1], color);
    DrawLineV(points[1], points[2], color);
    DrawLineV(points[2], points[0], color);
}

// Малює рівносторонній трикутник з обертанням і заливкою
void DrawFilledTriangleRotated(Vector2 center, float size, float rotationDeg, Color color) {
    float angleRad = rotationDeg * DEG2RAD;
    float height = (sqrtf(3.0f) / 2.0f) * size;  // Висота рівностороннього трикутника

    Vector2 points[3];

    // Побудова вершин рівностороннього трикутника зі стороною size
    points[0].x = 0;
    points[0].y = -2.0f / 3.0f * height;          // Верхня вершина

    points[1].x = -size / 2.0f;
    points[1].y = 1.0f / 3.0f * height;

    points[2].x = size / 2.0f;
    points[2].y = 1.0f / 3.0f * height;

    // Поворот і переміщення вершин відносно центру
    for (int i = 0; i < 3; i++) {
        float xRot = points[i].x * cosf(angleRad) - points[i].y * sinf(angleRad);
        float yRot = points[i].x * sinf(angleRad) + points[i].y * cosf(angleRad);
        points[i].x = center.x + xRot;
        points[i].y = center.y + yRot;
    }

    // Заливка трикутника
    FillTriangle(points[0], points[1], points[2], color);
}

