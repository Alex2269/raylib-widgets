// gui_dropdown_roller_advanced.h

#ifndef GUI_DROPDOWN_ROLLER_ADVANCED_H
#define GUI_DROPDOWN_ROLLER_ADVANCED_H

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
int Gui_Dropdown_Roller_Advanced(Rectangle bounds, const char **items, int itemCount,
                                 int currentIndex, const char *hintText, bool *isOpen, Color color,
                                 float *scrollOffset, float *scrollVelocity);

#endif // GUI_DROPDOWN_ROLLER_ADVANCED_H

