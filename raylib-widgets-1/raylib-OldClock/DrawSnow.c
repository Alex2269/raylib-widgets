// file DrawSnow.c
#include "DrawSnow.h"

// Рекурсивна функція для малювання однієї розгалуженої гілочки
void DrawSnowflakeBranch(Vector2 startPoint, float length, float angle, int depth, float branchAngle, float branchScale, Color color) {
    if (depth == 0) return; // Базовий випадок рекурсії

    Vector2 endPoint = {
        startPoint.x + cos(angle) * length,
        startPoint.y + sin(angle) * length
    };

    DrawLineEx(startPoint, endPoint, 1.5f, color);

    // Рекурсивно малюємо дві менші гілочки від кінця поточної
    float nextLength = length * branchScale;
    int nextDepth = depth - 1;

    // Ліва гілочка
    DrawSnowflakeBranch(endPoint, nextLength, angle + branchAngle, nextDepth, branchAngle, branchScale, color);

    // Права гілочка
    DrawSnowflakeBranch(endPoint, nextLength, angle - branchAngle, nextDepth, branchAngle, branchScale, color);
}

// Функція для малювання всієї сніжинки з розгалуженими гілочками
void DrawComplexSnowflake(Vector2 center, float radius, int numArms, int depth, float branchAngle, float branchScale, Color color) {
    float angleStep = 360.0f / numArms;

    for (int i = 0; i < numArms; i++) {
        float currentAngle = angleStep * i * DEG2RAD; // Кут основного променя в радіанах
        Vector2 startPoint = center;
        DrawSnowflakeBranch(startPoint, radius, currentAngle, depth, branchAngle * DEG2RAD, branchScale, color);
    }
}

