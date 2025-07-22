// LoadFontUnicode.h

#ifndef LOADFONTUNICODE_H
#define LOADFONTUNICODE_H

#include "raylib.h"

// Функція завантажує шрифт з підтримкою Unicode.
// Параметри:
//   fnt - шлях до файлу шрифту (наприклад, "bold.ttf")
//   fontSize - розмір шрифту в пікселях
//   LineSpacing - відступ між рядками тексту (інтерліньяж)
// Повертає:
//   Об'єкт Font, який можна використовувати для малювання тексту з Unicode символами.
Font LoadFontUnicode(char * fnt, int fontSize, int LineSpacing)
{
    // Визначаємо кількість кодових точок (символів), які завантажимо у шрифт
    int codepointCount = 512;
    // Масив для зберігання кодів символів Unicode
    int codepoints[512] = { 0 };

    // Заповнюємо масив кодами ASCII символів від 32 (пробіл) до 126 (~)
    for (int i = 0; i < 95; i++) codepoints[i] = 32 + i;

    // Додаємо грецькі символи Unicode, починаючи з коду 928
    for (int i = 96; i < 351; i++) codepoints[i] = 928 + i;

    // Завантажуємо шрифт з вказаними кодовими точками
    Font font = LoadFontEx(fnt, fontSize, codepoints, codepointCount);

    // Встановлюємо білінійну фільтрацію текстури шрифту для покращення якості відображення
    SetTextureFilter(font.texture, TEXTURE_FILTER_BILINEAR);

    // Встановлюємо відступ між рядками тексту (інтерліньяж)
    SetTextLineSpacing(LineSpacing);

    return font;
}

#endif /* LOADFONTUNICODE_H */
