#include "DrawIsoscelesTriangle.h"

#define DEG2RAD (3.14159265f / 180.0f)

// Структура для сортування вершин за y
// typedef struct {
//     Vector2 v[3];
// } Triangle;

// Малює рівносторонній трикутник, повернутий на rotationDeg градусів
void DrawTriangleLinesRotated(Vector2 center, float size, float rotationDeg, Color color) {
    float angleRad = rotationDeg * DEG2RAD;

    // Кути вершин рівностороннього трикутника (120 градусів між вершинами)
    float angles[3] = {
        angleRad,
        angleRad + 2.0f * M_PI / 3.0f,
        angleRad + 4.0f * M_PI / 3.0f
    };

    // Тут size фактично є радіусом описаного кола навколо рівностороннього трикутника.
    Vector2 points[3];
    for (int i = 0; i < 3; i++) {
        points[i].x = center.x + size * cosf(angles[i]);
        points[i].y = center.y + size * sinf(angles[i]);
    }

    // Малюємо лінії між вершинами
    DrawLineV(points[0], points[1], color);
    DrawLineV(points[1], points[2], color);
    DrawLineV(points[2], points[0], color);
}

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
    // Сортуємо вершини по y (v0.y <= v1.y <= v2.y)
    Vector2 verts[3] = {v0, v1, v2};
    qsort(verts, 3, sizeof(Vector2), CompareY);

    v0 = verts[0];
    v1 = verts[1];
    v2 = verts[2];

    // EPSILON - дуже мале число, що використовується для уникнення проблем
    // з порівнянням чисел з плаваючою крапкою, особливо щоб уникнути
    // ділення на нуль або обробки "плоских" трикутників, де висота надто мала.
    const float EPSILON = 0.0001f;

    // Визначаємо загальну висоту трикутника по y
    float total_height = v2.y - v0.y;
    if (total_height < EPSILON) return; // Якщо висота дуже мала - вважаємо, що трикутник "плоский" і нічого не малюємо

    // Верхня частина трикутника
    float segment_height = v1.y - v0.y;
    if (segment_height > EPSILON) {
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
    if (segment_height > EPSILON) {
        for (int y = (int)ceilf(v1.y); y <= (int)floorf(v2.y); y++) {
            float alpha = (y - v0.y) / total_height;
            float beta = (y - v1.y) / segment_height;
            float Ax = m_Lerp(v0.x, v2.x, alpha);
            float Bx = m_Lerp(v1.x, v2.x, beta);
            DrawScanline(y, (int)(Ax + 0.5f), (int)(Bx + 0.5f), color);
        }
    }
}

// Малюємо рівносторонній трикутник з обертанням і заливкою
void DrawFilledTriangleRotated(Vector2 center, float size, float rotationDeg, Color color) {
    float angleRad = rotationDeg * DEG2RAD;

    // Тут size фактично є радіусом описаного кола навколо рівностороннього трикутника.
    Vector2 points[3];
    for (int i = 0; i < 3; i++) {
        float angle = angleRad + i * 2.0f * M_PI / 3.0f;
        points[i].x = center.x + size * cosf(angle);
        points[i].y = center.y + size * sinf(angle);
    }

    // Функція заливки трикутника за вершинами
    FillTriangle(points[0], points[1], points[2], color);
    // DrawTriangle(points[2], points[1], points[0], color);
}

