#ifndef GUI_SPINNER_H
#define GUI_SPINNER_H

#include "raylib.h"
#include "all_font.h"

#define MAX_SPINNERS 16

typedef enum {
    GUI_SPINNER_INT,
    GUI_SPINNER_FLOAT
} GuiSpinnerValueType;

typedef enum {
    GUI_SPINNER_HORIZONTAL,
    GUI_SPINNER_VERTICAL
} GuiSpinnerOrientation;

/**
 * Головна функція слайдера з підтримкою орієнтації
 */
bool Gui_Spinner(int id, int centerX, int centerY, int width, int height,
                 const char* textLeft, const char* textRight,
                 void* value, void* minValue, void* maxValue,
                 float step, GuiSpinnerValueType valueType,
                 GuiSpinnerOrientation orientation,
                 Color baseColor, RasterFont font, int spacing);

#endif // GUI_SPINNER_H
