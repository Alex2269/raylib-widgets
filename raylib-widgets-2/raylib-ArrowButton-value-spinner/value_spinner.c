// value_spinner.c

#include "value_spinner.h"
#include <stdio.h>

void ValueSpinner_Init(ValueSpinner *spinner, int x, int y, int btnWidth, int btnHeight,
                       int minValue, int maxValue, int step, int *value,
                       PSF_Font font, int spacing, Color colorUp, Color colorDown, Color bgColor) {
    spinner->btnUp = (Rectangle){ x, y, btnWidth, btnHeight };
    spinner->btnDown = (Rectangle){ x + btnWidth + 80, y, btnWidth, btnHeight }; // 80 - відстань між кнопками
    spinner->valueRect = (Rectangle){ x + btnWidth + 5, y, 80 - 10, btnHeight }; // фон між кнопками
    spinner->minValue = minValue;
    spinner->maxValue = maxValue;
    spinner->step = step;
    spinner->value = value;
    spinner->font = font;
    spinner->colorUp = colorUp;
    spinner->colorDown = colorDown;
    spinner->bgColor = bgColor;
    spinner->spacing = spacing;
    spinner->holdUp = (HoldState){0};
    spinner->holdDown = (HoldState){0};
}

bool ValueSpinner_UpdateDraw(ValueSpinner *spinner) {
    bool changed = false;

    // Малюємо кнопки зі стрілками
    if (Gui_ArrowButton(spinner->btnUp, spinner->font, ARROW_UP, true,
                        spinner->value, spinner->step, spinner->minValue, spinner->maxValue,
                        NULL, NULL, spinner->colorUp, &spinner->holdUp)) {
        changed = true;
    }

    if (Gui_ArrowButton(spinner->btnDown, spinner->font, ARROW_DOWN, true,
                        spinner->value, spinner->step, spinner->minValue, spinner->maxValue,
                        NULL, NULL, spinner->colorDown, &spinner->holdDown)) {
        changed = true;
    }

    // Малюємо фон для значення
    DrawRectangleRec(spinner->valueRect, spinner->bgColor);
    DrawRectangleLines(spinner->valueRect.x, spinner->valueRect.y, spinner->valueRect.width, spinner->valueRect.height, DARKGRAY);

    // Формуємо текст значення
    char valueText[32];
    sprintf(valueText, "%d", *spinner->value);
    int charCount = utf8_strlen(valueText);
    int textWidth = charCount * (spinner->font.width + spinner->spacing) - spinner->spacing;

    // Малюємо текст по центру valueRect
    float textX = spinner->valueRect.x + (spinner->valueRect.width - textWidth) / 2.0f;
    float textY = spinner->valueRect.y + (spinner->valueRect.height - spinner->font.height) / 2.0f;

    DrawPSFText(spinner->font, (int)textX, (int)textY, valueText, spinner->spacing, BLACK);

    return changed;
}

