// knob_gui_int.h

#ifndef KNOB_GUI_INT_H
#define KNOB_GUI_INT_H

#include "raylib.h"
#include <stdbool.h>

/**
 * @brief Відображає та обробляє поворотний регулятор (knob) для певного каналу з цілим значенням.
 *
 * @param channel Номер каналу (індекс від 0 до 3)
 * @param x_pos Координата X центру регулятора на екрані
 * @param y_pos Координата Y центру регулятора на екрані
 * @param textTop Текст підказки, що відображається зверху при наведенні
 * @param radius Радіус кола регулятора в пікселях
 * @param value Вказівник на значення регулятора типу int (в межах minValue..maxValue)
 * @param minValue Мінімальне значення регулятора (int)
 * @param maxValue Максимальне значення регулятора (int)
 * @param isActive Чи активний регулятор (чи реагує на події користувача)
 * @param colorText Колір тексту значення регулятора (тип Color з raylib)
 * @return int 1, якщо значення регулятора змінилося, інакше 0.
 */
int Gui_Knob_Channel_Int(int channel, int x_pos, int y_pos, const char *textTop, float radius,
                         int *value, int minValue, int maxValue, bool isActive, Color colorText);

#endif // KNOB_GUI_INT_H

