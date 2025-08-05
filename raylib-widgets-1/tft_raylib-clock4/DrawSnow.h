// file DrawSnow.h
#ifndef DRAWSNOW_H
#define DRAWSNOW_H

#include "raylib.h"

/*
    Пояснення параметрів:

    startPoint: Початкова точка гілочки.
    length: Довжина поточної гілочки.
    angle: Кут напрямку поточної гілочки в радіанах.
    depth: Поточний рівень рекурсії. Коли depth досягає 0, рекурсія зупиняється.
    branchAngle: Кут відхилення бічних гілочок від основної гілочки (в радіанах).
    branchScale: Коефіцієнт, на який множиться довжина гілочки на кожному наступному рівні рекурсії (значення менше 1 призводить до зменшення довжини).
    color: Колір сніжинки.
    numArms: Кількість основних променів сніжинки.
    radius: Довжина основних променів (початкова довжина гілочок).
    Експериментуйте зі значеннями depth, branchAngle та branchScale, щоб отримати різноманітні форми та рівні розгалуження вашої сніжинки! Більша глибина та менший коефіцієнт масштабування створять більш складні та деталізовані структури.
 */

// Рекурсивна функція для малювання однієї розгалуженої гілочки
void DrawSnowflakeBranch(Vector2 startPoint, float length, float angle, int depth, float branchAngle, float branchScale, Color color);
// Функція для малювання всієї сніжинки з розгалуженими гілочками
void DrawComplexSnowflake(Vector2 center, float radius, int numArms, int depth, float branchAngle, float branchScale, Color color);

#endif /* DRAWSNOW_H */

