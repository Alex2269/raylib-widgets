// gui_slider.c

#include "gui_slider.h"
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <math.h>

// Структура для збереження стану слайдера
typedef struct {
    Rectangle bounds; // Позиція та розмір слайдера на екрані
    bool isActive;    // Чи зараз слайдер перетягується мишею
    bool used;        // Чи використовується цей слот для слайдера
} SliderState;

#define MAX_SLIDERS 16

/**
 * Визначення контрастного кольору (чорний або білий),
 * залежно від яскравості базового кольору
 */
static Color GetContrastingColor(Color c) {
    float luminance = 0.2126f * c.r / 255 + 0.7152f * c.g / 255 + 0.0722f * c.b / 255;
    return (luminance > 0.5f) ? BLACK : WHITE;
}

// Статичний масив станів слайдерів за індексом ID
static SliderState slidersState[MAX_SLIDERS] = {{0}};

/**
 * Пошук або ініціалізація стану слайдера по його прямокутній області.
 * Якщо слайдер уже є в масиві, повертає вказівник на його isActive.
 * Якщо ні — ініціалізує новий слот і повертає вказівник.
 * Якщо перевищено ліміт слайдерів — повертає NULL.
 */
static bool* GetSliderActiveState(Rectangle bounds) {
    for (int i = 0; i < MAX_SLIDERS; i++) {
        if (slidersState[i].used) {
            // Перевірка, чи це той самий слайдер (з такими ж межами)
            if (memcmp(&slidersState[i].bounds, &bounds, sizeof(Rectangle)) == 0) {
                return &slidersState[i].isActive;
            }
        } else {
            // Вільний слот — ініціалізуємо слайдер
            slidersState[i].bounds = bounds;
            slidersState[i].isActive = false;
            slidersState[i].used = true;
            return &slidersState[i].isActive;
        }
    }
    // Перевищено максимальну кількість слайдерів
    return NULL;
}

/**
 * Малювання слайдера із заповненням по нормалізованій позиції (normPos в діапазоні [0..1]).
 *
 * @param bounds - прямокутник слайдера
 * @param normPos - нормалізоване значення (0 - мінімум, 1 - максимум)
 * @param baseColor - основний колір елемента
 */
static void DrawSlider(Rectangle bounds, float normPos, Color baseColor) {
    // Фон слайдера – світлий і прозорий для основного кольору
    DrawRectangleRec(bounds, Fade(baseColor, 0.25f));
    // Заповнення слайдера відповідно до поточного значення
    DrawRectangle(bounds.x, bounds.y, normPos * bounds.width, bounds.height, Fade(baseColor, 0.5f));
    // При бажанні - розкоментувати фонову рамку
    // DrawRectangleLinesEx(bounds, 1, GetContrastingColor(baseColor)); // Фонова рамка

    // Можна домалювати ручку слайдера (по центру заповнення)
    float knobX = bounds.x + normPos * bounds.width;
    float knobW = 10, knobH = bounds.height;
    Rectangle knobRect = {knobX - knobW / 2, bounds.y, knobW, knobH};
    // При бажанні - розкоментувати для видимості ручки
    // DrawRectangleRec(knobRect, GetContrastingColor(baseColor));
    // DrawRectangleLinesEx(knobRect, 1, baseColor);
}

/**
 * Оновлення значення на основі позиції миші щодо слайдера.
 * Значення нормалізується в діапазон [minValue..maxValue] з кроком step.
 */
static void UpdateSliderValue(Rectangle bounds, void* value, void* minVal, void* maxVal,
                              float step, GuiSliderValueType valueType)
{
    Vector2 mousePos = GetMousePosition();

    // Обчислення нормалізованої позиції миші по горизонталі
    float norm = (mousePos.x - bounds.x) / bounds.width;
    if (norm < 0) norm = 0;
    if (norm > 1) norm = 1;

    if (valueType == GUI_SLIDER_FLOAT) {
        float minV = *(float*)minVal;
        float maxV = *(float*)maxVal;
        float range = maxV - minV;

        // Обчислення "сирого" значення відносно мінімуму за позицією норми
        float rawVal = minV + norm * range;
        // Округлення до кроку за допомогою roundf
        float steppedVal = minV + step * roundf((rawVal - minV) / step);

        // Застосування обмежень меж
        if (steppedVal < minV) steppedVal = minV;
        if (steppedVal > maxV) steppedVal = maxV;

        *(float*)value = steppedVal;
    } else {
        int minV = *(int*)minVal;
        int maxV = *(int*)maxVal;
        int range = maxV - minV;

        int rawVal = minV + (int)(norm * range);

        int stepInt = (int)(step + 0.5f);
        if (stepInt == 0) stepInt = 1;

        // Округлення до найближчого кратного кроку
        int steppedVal = minV + ((rawVal - minV + stepInt / 2) / stepInt) * stepInt;

        // Обмеження в межах
        if (steppedVal < minV) steppedVal = minV;
        if (steppedVal > maxV) steppedVal = maxV;

        *(int*)value = steppedVal;
    }
}

/**
 * Основна функція віджета слайдера, з підтримкою горизонтальної та вертикальної орієнтації.
 *
 * @param id - Ідентифікатор віджета для збереження стану.
 * @param centerX, centerY - Координати центру слайдера.
 * @param width, height - Розмір області слайдера.
 * @param textLeft, textRight - Тексти над кнопками (можна NULL).
 * @param value - Вказівник на число, яке редагується.
 * @param minValue, maxValue - Вказівники на мінімальне та максимально допустиме значення.
 * @param step - Крок зміни.
 * @param valueType - Тип значення (int або float).
 * @param orientation - Горизонтальний або вертикальний.
 * @param baseColor - Основний колір.
 * @param font - Шрифт.
 * @param spacing - Відстань між символами.
 *
 * @return true, якщо значення змінилося.
 */
bool Gui_Slider(int id, int centerX, int centerY, int width, int height,
                const char* textLeft, const char* textRight,
                void* value, void* minValue, void* maxValue,
                float step, GuiSliderValueType valueType,
                GuiSliderOrientation orientation,
                Color baseColor, RasterFont font, int spacing)
{
    if (id < 0 || id >= 16 || !value) return false; // Перевірка валідності

    bool changed = false;

    // Обчислюємо верхній лівий кут залежно від орієнтації
    int posX = centerX - width / 2;
    int posY = centerY - height / 2;

    Rectangle sliderRect;

    if (orientation == GUI_SLIDER_HORIZONTAL) {
        // Горизонтальна орієнтація
        sliderRect = (Rectangle){(float)posX, (float)posY, (float)width, (float)height};
    } else {
        // Вертикальна орієнтація - ширина і висота міняються місцями
        sliderRect = (Rectangle){(float)posX, (float)posY, (float)height, (float)width};
    }

    // Обчислюємо нормальне значення (0..1)
    float normVal = 0.0f;
    if (valueType == GUI_SLIDER_FLOAT) {
        float v = *(float*)value;
        float minV = *(float*)minValue;
        float maxV = *(float*)maxValue;
        normVal = (maxV == minV) ? 0.0f : (v - minV) / (maxV - minV);
    } else {
        int v = *(int*)value;
        int minV = *(int*)minValue;
        int maxV = *(int*)maxValue;
        normVal = (maxV == minV) ? 0.0f : ((float)(v - minV) / (maxV - minV));
    }

    // Обмежуємо межі
    if (normVal < 0) normVal = 0;
    if (normVal > 1) normVal = 1;

    if (orientation == GUI_SLIDER_VERTICAL) {
        posX = centerX - height / 2;
        posY = centerY - width / 2;
        sliderRect = (Rectangle){ (float)posX, (float)posY, (float)height, (float)width };
    } else {
        posX = centerX - width / 2;
        posY = centerY - height / 2;
        sliderRect = (Rectangle){ (float)posX, (float)posY, (float)width, (float)height };
    }

    // Малюємо слайдер згідно з орієнтацією
    if (orientation == GUI_SLIDER_HORIZONTAL) {
        DrawSlider(sliderRect, normVal, baseColor);
    } else {
        // Вертикальний слайдер - заповнення знизу вгору
        DrawRectangleRec(sliderRect, Fade(baseColor, 0.25f));
        float fillHeight = normVal * sliderRect.height;
        DrawRectangle(sliderRect.x, sliderRect.y + sliderRect.height - fillHeight,
                      sliderRect.width, fillHeight, Fade(baseColor, 0.5f));

        // Малюємо ручку на рівні заповнення
        float knobY = sliderRect.y + sliderRect.height - fillHeight;
        float knobW = sliderRect.width;
        float knobH = 4;
        Rectangle knobRect = {sliderRect.x, knobY - knobH/2, knobW, knobH};
        // Можна додати малювання ручки, якщо потрібно
        // DrawRectangleRec(knobRect, GetContrastingColor(baseColor));
    }

    // Отримуємо стан активності (перетягування)
    bool *isActive = GetSliderActiveState(sliderRect);
    if (!isActive) return false;

    Vector2 mousePos = GetMousePosition();
    bool mouseOver = CheckCollisionPointRec(mousePos, sliderRect);

    // Обробка колеса миші
    if (mouseOver) {
        int wheelMove = GetMouseWheelMove();
        if (wheelMove != 0) {
            if (valueType == GUI_SLIDER_FLOAT) {
                float* v = (float*)value;
                float maxV = *(float*)maxValue;
                float minV = *(float*)minValue;
                *v += step * wheelMove;
                if (*v > maxV) *v = maxV;
                if (*v < minV) *v = minV;
            } else {
                int* v = (int*)value;
                int maxV = *(int*)maxValue;
                int minV = *(int*)minValue;
                *v += (int)(step) * wheelMove;
                if (*v > maxV) *v = maxV;
                if (*v < minV) *v = minV;
            }
            changed = true;
        }
    }

    // Початок перетягування
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && mouseOver) {
        *isActive = true;
    }
    // Завершення перетягування
    if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
        *isActive = false;
    }

    // Оновлюємо значення, коли перетягуємо
    if (*isActive) {
        bool valueChanged = false;
        if (orientation == GUI_SLIDER_HORIZONTAL) {
            // Горизонтальне оновлення
            float oldVal = (valueType == GUI_SLIDER_FLOAT) ? *(float*)value : 0;
            int oldValInt = (valueType == GUI_SLIDER_INT) ? *(int*)value : 0;
            UpdateSliderValue(sliderRect, value, minValue, maxValue, step, valueType);
            if (valueType == GUI_SLIDER_FLOAT && *(float*)value != oldVal) valueChanged = true;
            if (valueType == GUI_SLIDER_INT && *(int*)value != oldValInt) valueChanged = true;
        } else {
            // Вертикальне оновлення: враховуємо інвертовану ось
            float norm = 1.0f - (mousePos.y - sliderRect.y) / sliderRect.height;
            if (norm < 0) norm = 0;
            if (norm > 1) norm = 1;

            if (valueType == GUI_SLIDER_FLOAT) {
                float minV = *(float*)minValue;
                float maxV = *(float*)maxValue;
                float rawVal = minV + norm * (maxV - minV);
                float steppedVal = minV + step * roundf((rawVal - minV) / step);
                if (steppedVal < minV) steppedVal = minV;
                if (steppedVal > maxV) steppedVal = maxV;
                if (steppedVal != *(float*)value) {
                    *(float*)value = steppedVal;
                    valueChanged = true;
                }
            } else {
                int minV = *(int*)minValue;
                int maxV = *(int*)maxValue;
                int range = maxV - minV;
                int rawVal = minV + (int)(norm * range);
                int stepInt = (int)(step + 0.5f);
                if (stepInt == 0) stepInt = 1;
                int steppedVal = minV + ((rawVal - minV + stepInt / 2) / stepInt) * stepInt;
                if (steppedVal < minV) steppedVal = minV;
                if (steppedVal > maxV) steppedVal = maxV;
                if (steppedVal != *(int*)value) {
                    *(int*)value = steppedVal;
                    valueChanged = true;
                }
            }
        }
        if (valueChanged) changed = true;
    }

    // Відображення поточного значення
    char valStr[32];
    if (valueType == GUI_SLIDER_FLOAT)
        snprintf(valStr, sizeof(valStr), "%.2f", *(float*)value);
    else
        snprintf(valStr, sizeof(valStr), "%d", *(int*)value);

    // Обчислюємо позицію для тексту
    int textLen = 0;
    for (const char *p=valStr; *p; p++) if (((*p) & 0xC0) != 0x80) textLen++;
    int textWidth = textLen * (font.glyph_width + spacing) - spacing;
    int textHeight = font.glyph_height;

    float centerXf = sliderRect.x + sliderRect.width / 2.0f;
    float centerYf = sliderRect.y + sliderRect.height / 2.0f;

    const int padX = 8; // Відступ зліва/зправа
    const int padY = 4; // Відступ зверху/знизу

    Rectangle valBgRect = {
        // Центруємо текст по горизонталі та вертикалі
        (float)(centerXf - textWidth / 2 - padX),
        (float)(centerYf - textHeight / 2 - padY),
        (float)(textWidth + 2 * padX),
        (float)(textHeight + 2 * padY)
    };

    // Малюємо фон для значення
    Color valBgColor = Fade(baseColor, 0.2f);
    Color valFgColor = GetContrastingColor(valBgColor);

    // Можна розкоментувати для візуалізації
    // DrawRectangleRec(valBgRect, valBgColor);
    // DrawRectangleLinesEx(valBgRect, 1, valFgColor);

    // Відображення тексту
    int textDrawX = (int)(valBgRect.x + padX);
    int textDrawY = (int)(valBgRect.y + padY);
    DrawTextScaled(font, textDrawX, textDrawY, valStr, spacing, 1, valFgColor);

    return changed; // Повертає true якщо значення змінилося
}

