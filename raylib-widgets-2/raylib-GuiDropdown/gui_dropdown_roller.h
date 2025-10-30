// gui_dropdown_roller.h

#ifndef GUI_DROPDOWN_ROLLER_H
#define GUI_DROPDOWN_ROLLER_H

#include "raylib.h"
#include <stdbool.h>

/**
 * Функція малює dropdown список.
 *
 * @param bounds - позиція і розмір кнопки dropdown
 * @param items - масив рядків пунктів
 * @param itemCount - кількість пунктів
 * @param currentIndex - поточний вибір (індекс)
 * @param hintText - текст підказки (над кнопкою), може бути NULL
 * @param isOpen - вказівник на булевий стан відкриття списку
 * @param color - колір активного елемента
 *
 * @return оновлений індекс вибраного пункту
 */
int Gui_Dropdown_Roller(Rectangle bounds, const char **items, int itemCount,
                        int currentIndex, const char *hintText, bool *isOpen, Color color,
                        float *scrollOffset);

#endif // GUI_DROPDOWN_ROLLER_H

