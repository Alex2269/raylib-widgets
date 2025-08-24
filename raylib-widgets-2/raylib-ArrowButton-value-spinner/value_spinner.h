// value_spinner.h

#ifndef VALUE_SPINNER_H
#define VALUE_SPINNER_H

#include "raylib.h"
#include "psf_font.h"
#include "arrow_button.h"

typedef struct {
    Rectangle btnUp;
    Rectangle btnDown;
    Rectangle valueRect;
    int minValue;
    int maxValue;
    int step;
    int *value;
    PSF_Font font;
    Color colorUp;
    Color colorDown;
    Color bgColor;
    HoldState holdUp;
    HoldState holdDown;
    int spacing;
} ValueSpinner;

// Ініціалізація віджета (передача позиції, розмірів, шрифту, кольорів, змінної значення)
void ValueSpinner_Init(ValueSpinner *spinner, int x, int y, int btnWidth, int btnHeight,
                       int minValue, int maxValue, int step, int *value,
                       PSF_Font font, int spacing, Color colorUp, Color colorDown, Color bgColor);

// Малювання і оновлення віджета, повертає true, якщо значення змінилось
bool ValueSpinner_UpdateDraw(ValueSpinner *spinner);

#endif // VALUE_SPINNER_H
