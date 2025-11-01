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
    DrawRectangleLinesEx(bounds, 1, GetContrastingColor(baseColor)); // Рамка

    // Можна домалювати ручку слайдера (по центру заповнення)
    float knobX = bounds.x + normPos * bounds.width;
    float knobW = 10, knobH = bounds.height;
    Rectangle knobRect = {knobX - knobW / 2, bounds.y, knobW, knobH};
    // При бажанні - розкоментувати для видимості ручки
    DrawRectangleRec(knobRect, GetContrastingColor(baseColor));
    DrawRectangleLinesEx(knobRect, 1, baseColor);
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
 * Функція віджета слайдера з відображенням і взаємодією.
 * Підтримує typen int/float, зміну значення через перетягування і колесо миші.
 */
bool Gui_Slider(int id, int centerX, int centerY, int width, int height,
                const char* textLeft, const char* textRight,
                void* value, void* minValue, void* maxValue,
                float step, GuiSliderValueType valueType,
                Color baseColor, RasterFont font, int spacing)
{
    // Перевірка валідності id і вказівника
    if (id < 0 || id >= MAX_SLIDERS || !value) return false;

    bool changed = false; // Прапорець, чи було змінено значення

    // Обчислення позиції прямокутника слайдера (повна ширина без кнопок)
    int posY = centerY - height / 2;
    Rectangle sliderRect = {(float)(centerX - width / 2), (float)posY, (float)width, (float)height};

    // Обрахунок нормалізованого значення від 0 до 1 для поточного value
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
        normVal = (maxV == minV) ? 0.0f : ((float)(v - minV) / (float)(maxV - minV));
    }
    if (normVal < 0) normVal = 0;
    if (normVal > 1) normVal = 1;

    // Малюємо слайдер з відображенням рівня заповнення
    DrawSlider(sliderRect, normVal, baseColor);

    // Отримуємо внутрішній стан "активності" слайдера за прямокутником
    bool *isActive = GetSliderActiveState(sliderRect);
    if (!isActive) return false; // Якщо максимальна кількість слайдерів перевищена

    // Отримуємо позицію миші і перевіряємо, чи наведена миша на слайдер
    Vector2 mousePos = GetMousePosition();
    bool mouseOver = CheckCollisionPointRec(mousePos, sliderRect);

    // Зміна значення колесом миші, якщо курсор на слайдері
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
                changed = true;
            } else {
                int* v = (int*)value;
                int maxV = *(int*)maxValue;
                int minV = *(int*)minValue;
                *v += (int)(step) * wheelMove;
                if (*v > maxV) *v = maxV;
                if (*v < minV) *v = minV;
                changed = true;
            }
        }
    }

    // Якщо натиснуто ліву кнопку миші на слайдері - активуємо перетягування
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && mouseOver) {
        *isActive = true;
    }
    // Якщо відпущено кнопку миші - відключаємо перетягування
    if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
        *isActive = false;
    }

    // Якщо слайдер утримується (перетягується)
    if (*isActive) {
        bool valueChanged = false;
        if (valueType == GUI_SLIDER_FLOAT) {
            float oldVal = *(float*)value;
            UpdateSliderValue(sliderRect, value, minValue, maxValue, step, valueType);
            if (oldVal != *(float*)value) valueChanged = true;
        } else {
            int oldVal = *(int*)value;
            UpdateSliderValue(sliderRect, value, minValue, maxValue, step, valueType);
            if (oldVal != *(int*)value) valueChanged = true;
        }
        if (valueChanged) changed = true;
    }

    // Відображення поточного значення зверху по центру слайдера
    char valStr[32];
    if (valueType == GUI_SLIDER_FLOAT)
        snprintf(valStr, sizeof(valStr), "%.2f", *(float*)value);
    else
        snprintf(valStr, sizeof(valStr), "%d", *(int*)value);

    int textLen = 0;
    for (const char* p = valStr; *p; p++) if (((*p) & 0xC0) != 0x80) textLen++;
    int textWidth = textLen * (font.glyph_width + spacing) - spacing;
    int textHeight = font.glyph_height;

    float centerXf = sliderRect.x + sliderRect.width / 2.0f;
    float centerYf = sliderRect.y + sliderRect.height / 2.0f;

    const int padX = 8;
    const int padY = 4;

    Rectangle valBgRect = {
        centerXf - (textWidth / 2.0f) - padX,
        centerYf - (textHeight / 2.0f) - padY,
        textWidth + 2 * padX,
        textHeight + 2 * padY
    };

    Color valBgColor = Fade(baseColor, 0.2f);
    Color valFgColor = GetContrastingColor(valBgColor);

    // За бажанням можна розкоментувати фон і рамку тексту
    // DrawRectangleRec(valBgRect, valBgColor);
    // DrawRectangleLinesEx(valBgRect, 1, valFgColor);

    int textDrawX = (int)(valBgRect.x + padX);
    int textDrawY = (int)(valBgRect.y + padY);

    DrawTextScaled(font, textDrawX, textDrawY, valStr, spacing, 1, valFgColor);

    return changed;
}

