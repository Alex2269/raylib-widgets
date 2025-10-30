// LoadFontUnicode.h

#ifndef LOADFONTUNICODE_H
#define LOADFONTUNICODE_H

/**
 * Функція завантаження шрифту з підтримкою Unicode.
 * Підтримує символи латиниці та кирилиці.
 *
 * @param fnt Путь до файлу шрифту
 * @param fontSize Розмір шрифту
 * @param LineSpacing Відстань між рядками для мультистрокового тексту
 * @return Font Ініціалізований шрифт із встановленими параметрами
 */
Font LoadFontUnicode(char * fnt, int fontSize, int LineSpacing)
{
    // Кількість символів, які будуть підтримуватися у шрифті (512 символів)
    const int codepointCount = 512;
    int codepoints[512] = { 0 };

    // Заповнюємо кодпоінти: базова латиниця (32..126), кирилиця (928..1182)
    for (int i = 0; i < 95; i++) codepoints[i] = 32 + i;
    for (int i = 96; i < 351; i++) codepoints[i] = 928 + i;

    // Завантажуємо шрифт з зазначеними кодпоінтами
    Font font = LoadFontEx(fnt, fontSize, codepoints, codepointCount);

    // Встановлення білінійної фільтрації для згладжування текстури шрифту
    SetTextureFilter(font.texture, TEXTURE_FILTER_BILINEAR);

    // Встановлення міжрядкових відступів для можливості мультистрокового тексту
    SetTextLineSpacing(LineSpacing);

    return font;
}

#endif /* LOADFONTUNICODE_H */

