// button.h
#ifndef BUTTON_EXTENDED_H
#define BUTTON_EXTENDED_H

#include "raylib.h"
#include <stdbool.h>

#include "all_font.h" // Опис шрифтів як структури RasterFont
#include "color_utils.h"
#include "glyphs.h"

// Структура стану кнопки для long press і double click
typedef struct {
    double lastClickTime;       // Час останнього кліку
    double longPressStartTime;  // Час початку тривалого натискання
    bool isPressed;             // Чи натиснута кнопка зараз
    bool longPressTriggered;    // Чи було уже тригернуто тривале натискання
} ButtonState;

/**
 * Функція кнопки з підтримкою:
 * - звичайного кліку,
 * - тривалого натискання (long press),
 * - подвійного кліку (double click).
 *
 * @param bounds - область кнопки
 * @param font - шрифт для тексту
 * @param text - текст кнопки
 * @param spacing - відступ між символами тексту кнопки
 * @param colorNormal - колір кнопки в нормальному стані
 * @param colorHover - колір кнопки при наведенні
 * @param colorPressed - колір кнопки при натисканні
 * @param colorText - колір тексту (якщо альфа 0, автоматично контрастний)
 * @param state - вказівник на структуру стану кнопки (для відстеження кліків)
 * @param outLongPress - вихідний параметр, встановлюється true при тривалому натисканні
 * @param outDoubleClick - вихідний параметр, встановлюється true при подвійний клік
 * @return true, якщо було одиничне натискання (відпускання кнопки)
 */
bool Gui_ButtonExtended(Rectangle bounds, RasterFont font, const char *text, int spacing,
                        Color colorNormal, Color colorHover, Color colorPressed,
                        Color colorText, ButtonState *state,
                        bool *outLongPress, bool *outDoubleClick);

#endif // BUTTON_EXTENDED_H
