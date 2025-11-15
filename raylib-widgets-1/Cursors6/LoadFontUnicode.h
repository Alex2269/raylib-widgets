// LoadFontUnicode.h

#ifndef LOADFONTUNICODE_H
#define LOADFONTUNICODE_H

#include "raylib.h"

/*
 * Завантажує шрифт із підтримкою Unicode.
 *
 * Параметри:
 *   fnt - шлях до файлу шрифту (TTF)
 *   fontSize - розмір шрифту
 *   LineSpacing - міжрядковий інтервал для тексту
 *
 * Повертає:
 *   Об'єкт Font з завантаженим шрифтом
 */
Font LoadFontUnicode(char * fnt, int fontSize, int LineSpacing)
{
    int codepointCount = 512;
    int codepoints[512] = { 0 };

    // Ініціалізація масиву кодових точок Unicode для завантаження символів
    for (int i = 0; i < 95; i++) codepoints[i] = 32 + i;      // ASCII символи від 32 до 126
    for (int i = 96; i < 351; i++) codepoints[i] = 928 + i;   // Додаткові символи (наприклад, кирилиця)

    Font font = LoadFontEx(fnt, fontSize, codepoints, codepointCount);

    // Встановлення білінійної фільтрації для текстури шрифту (плавне масштабування)
    SetTextureFilter(font.texture, TEXTURE_FILTER_BILINEAR);

    // Встановлення міжрядкового інтервалу для багаторядкового тексту
    SetTextLineSpacing(LineSpacing);

    return font;
}

#endif /* LOADFONTUNICODE_H */

