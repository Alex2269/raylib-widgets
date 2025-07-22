// sliders.c

#include "raylib.h"
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

#include "psf_font.h"  // Підключення PSF шрифтів
extern PSF_Font font18;  // Зовнішній шрифт для тексту
extern int spacing;      // Відступ між символами у тексті

#define SLIDER_KNOB_SIZE 12    // Розмір ручки слайдера (ширина або висота залежно від орієнтації)
#define MAX_SLIDERS 10         // Максимальна кількість слайдерів, які одночасно можуть існувати

// Структура для збереження стану кожного слайдера
typedef struct {
    Rectangle bounds;  // Прямокутник слайдера (позиція і розмір)
    bool isActive;     // Чи активний слайдер (перетягується зараз)
    bool used;         // Чи використовується цей слот для слайдера
} SliderState;

// Масив станів слайдерів
static SliderState slidersState[MAX_SLIDERS] = {0};

// Індекс слайдера, який зараз перетягується (-1, якщо перетягування відсутнє)
static int activeDraggingSlider = -1;

// Пошук або ініціалізація стану слайдера за його bounds
static bool* GetSliderActiveState(Rectangle bounds) {
    for (int i = 0; i < MAX_SLIDERS; i++) {
        if (slidersState[i].used) {
            // Якщо слайдер з такими ж bounds вже є, повертаємо вказівник на його isActive
            if (memcmp(&slidersState[i].bounds, &bounds, sizeof(Rectangle)) == 0) {
                return &slidersState[i].isActive;
            }
        } else {
            // Якщо слот вільний, ініціалізуємо його і повертаємо вказівник на isActive
            slidersState[i].bounds = bounds;
            slidersState[i].isActive = false;
            slidersState[i].used = true;
            return &slidersState[i].isActive;
        }
    }
    // Якщо всі слоти зайняті — повертаємо NULL
    return NULL;
}

// Обчислення яскравості кольору (luminance) для вибору контрастного тексту
static float GetLuminance(Color color) {
    float r = color.r / 255.0f;
    float g = color.g / 255.0f;
    float b = color.b / 255.0f;
    return 0.2126f * r + 0.7152f * g + 0.0722f * b;
}

// Вибір контрастного кольору тексту (чорний або білий) залежно від фону
static Color GetContrastingTextColor(Color bgColor) {
    return (GetLuminance(bgColor) > 0.5f) ? BLACK : WHITE;
}
/*
// Обчислення кількості символів у UTF-8 рядку
static int utf8_strlen(const char* s) {
    int len = 0;
    while (*s) {
        if ((*s & 0xc0) != 0x80) len++;
        s++;
    }
    return len;
}*/

// Головна функція слайдера
// bounds - прямокутник слайдера
// font - шрифт для тексту
// textTop - текст підказки над слайдером (може бути NULL)
// textRight - текст праворуч від слайдера (може бути NULL)
// value - вказівник на поточне значення слайдера
// minValue, maxValue - межі значення слайдера
// isVertical - орієнтація слайдера (true - вертикальний, false - горизонтальний)
// baseColor - базовий колір слайдера
float Gui_Slider(Rectangle bounds, PSF_Font font, const char *textTop, const char *textRight,
                 float *value, float minValue, float maxValue, bool isVertical, Color baseColor) {
    // Отримуємо вказівник на стан активності слайдера
    bool *isActive = GetSliderActiveState(bounds);
    if (isActive == NULL) return *value; // Якщо не знайшли слот, повертаємо поточне значення

    Vector2 mousePos = GetMousePosition();  // Позиція миші

    // Нормалізоване значення слайдера від 0 до 1
    float normValue = (*value - minValue) / (maxValue - minValue);
    if (normValue < 0) normValue = 0;
    if (normValue > 1) normValue = 1;

    // Обчислення прямокутника ручки слайдера (knob)
    Rectangle knobRect;
    if (isVertical) {
        // Для вертикального слайдера ручка рухається по осі Y
        float knobY = bounds.y + (1.0f - normValue) * bounds.height;
        knobRect = (Rectangle){ bounds.x, knobY - SLIDER_KNOB_SIZE/2, bounds.width, SLIDER_KNOB_SIZE };
    } else {
        // Для горизонтального слайдера ручка рухається по осі X
        float knobX = bounds.x + normValue * bounds.width;
        knobRect = (Rectangle){ knobX - SLIDER_KNOB_SIZE/2, bounds.y, SLIDER_KNOB_SIZE, bounds.height };
    }

    // Створюємо розширену область прилипання (sticky area) ±10 пікселів по напрямку руху ручки
    Rectangle stickyRect = knobRect;
    const int stickyMargin = 10;  // Розмір області прилипання

    if (isVertical) {
        // Розширюємо область по вертикалі: вгору і вниз на 10 пікселів
        stickyRect.y -= stickyMargin;
        stickyRect.height += 2 * stickyMargin;
    } else {
        // Розширюємо область по горизонталі: вліво і вправо на 10 пікселів
        stickyRect.x -= stickyMargin;
        stickyRect.width += 2 * stickyMargin;
    }

    // Перевірка, чи знаходиться курсор миші в області прилипання ручки
    bool mouseOverSticky = CheckCollisionPointRec(mousePos, stickyRect);

    // Активуємо перетягування, якщо натиснута ліва кнопка миші і курсор у зоні прилипання ручки,
    // і при цьому зараз не перетягується інший слайдер
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && mouseOverSticky && activeDraggingSlider == -1) {
        *isActive = true;  // Встановлюємо слайдер активним (перетягується)
        // Знаходимо індекс активного слайдера в масиві
        for (int i = 0; i < MAX_SLIDERS; i++) {
            if (slidersState[i].used && memcmp(&slidersState[i].bounds, &bounds, sizeof(Rectangle)) == 0) {
                activeDraggingSlider = i;
                break;
            }
        }
    }

    // Якщо відпущена ліва кнопка миші і слайдер активний — завершуємо перетягування
    if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON) && *isActive) {
        *isActive = false;
        activeDraggingSlider = -1;
    }

    // Якщо слайдер активний (перетягується), оновлюємо значення слайдера відповідно до позиції миші
    if (*isActive && activeDraggingSlider != -1) {
        if (isVertical) {
            // Для вертикального слайдера інвертуємо координату Y, щоб верх відповідав maxValue
            normValue = 1.0f - (mousePos.y - bounds.y) / bounds.height;
        } else {
            // Для горизонтального слайдера беремо координату X
            normValue = (mousePos.x - bounds.x) / bounds.width;
        }
        // Обмежуємо нормалізоване значення в межах [0,1]
        if (normValue < 0) normValue = 0;
        if (normValue > 1) normValue = 1;

        // Обчислюємо реальне значення слайдера з урахуванням меж
        *value = minValue + normValue * (maxValue - minValue);
    }

    // Визначаємо колір фону слайдера з урахуванням активності і наведення миші
    Color sliderBgColor = baseColor;
    if (!(*isActive)) sliderBgColor = Fade(sliderBgColor, 0.4f);  // Зменшуємо яскравість, якщо не активний
    if (CheckCollisionPointRec(mousePos, bounds)) sliderBgColor = Fade(baseColor, 0.8f);  // Трохи яскравіший при наведенні

    // Визначаємо контрастні кольори для рамки і тексту
    Color borderColor = GetContrastingTextColor(sliderBgColor);
    Color textColor = GetContrastingTextColor(borderColor);
    textColor.a = 255;  // Повна непрозорість тексту

    // Малюємо фон слайдера
    DrawRectangleRec(bounds, sliderBgColor);

    // Малюємо подвійний контур слайдера (внутрішній і зовнішній)
    float innerBorderThickness = 2.0f;
    float outerBorderThickness = 2.0f;

    Color innerBorderColor = GetContrastingTextColor(sliderBgColor);
    Color outerBorderColor = (innerBorderColor.r == 0 && innerBorderColor.g == 0 && innerBorderColor.b == 0) ? WHITE : BLACK;

    DrawRectangleLinesEx((Rectangle){ bounds.x - innerBorderThickness, bounds.y - innerBorderThickness,
                                    bounds.width + 2*innerBorderThickness, bounds.height + 2*innerBorderThickness },
                        innerBorderThickness, innerBorderColor);

    DrawRectangleLinesEx((Rectangle){ bounds.x - innerBorderThickness - outerBorderThickness, bounds.y - innerBorderThickness - outerBorderThickness,
                                    bounds.width + 2*(innerBorderThickness + outerBorderThickness), bounds.height + 2*(innerBorderThickness + outerBorderThickness) },
                        outerBorderThickness, outerBorderColor);

    // Малюємо ручку слайдера (knob)
    Color knobColor = *isActive ? GetContrastingTextColor(baseColor) : Fade(GetContrastingTextColor(baseColor), 0.5f);
    if (isVertical) {
        float knobY = bounds.y + (1.0f - normValue) * bounds.height;
        DrawRectangle(bounds.x, knobY - SLIDER_KNOB_SIZE/2, bounds.width, SLIDER_KNOB_SIZE, knobColor);
    } else {
        float knobX = bounds.x + normValue * bounds.width;
        DrawRectangle(knobX - SLIDER_KNOB_SIZE/2, bounds.y, SLIDER_KNOB_SIZE, bounds.height, knobColor);
    }

    // Малюємо підказку textTop над слайдером, якщо курсор над слайдером і текст заданий
    if (CheckCollisionPointRec(mousePos, bounds) && textTop && textTop[0] != '\0') {
        int padding = 6;  // Відступи всередині рамки
        int charCount = utf8_strlen(textTop);
        float textWidth = charCount * (font.width + spacing) - spacing;
        float boxWidth = textWidth + 2 * padding;
        float boxHeight = font.height + 2 * padding;

        // Розміщуємо рамку підказки по центру слайдера над ним
        Rectangle tooltipRect = {
            bounds.x + bounds.width / 2.0f - boxWidth / 2.0f,
            bounds.y - boxHeight - 8,
            boxWidth,
            boxHeight
        };

        DrawRectangleRec(tooltipRect, Fade(borderColor, 0.9f));
        DrawRectangleLinesEx(tooltipRect, 1, textColor);

        DrawPSFText(font,
                    tooltipRect.x + padding,
                    tooltipRect.y + padding / 2,
                    textTop, spacing, textColor);
    }

    // Малюємо текст праворуч від слайдера, якщо він заданий
    if (textRight && textRight[0] != '\0') {
        int padding = 6;
        int charCount = utf8_strlen(textRight);
        float textWidth = charCount * (font.width + spacing) - spacing;
        float boxWidth = textWidth + 2 * padding;
        float boxHeight = font.height + 2 * padding;

        Rectangle textRightRect = {
            bounds.x + bounds.width + 12,
            bounds.y + bounds.height / 2.0f - boxHeight / 2.0f,
            boxWidth,
            boxHeight
        };

        DrawRectangleRec(textRightRect, Fade(borderColor, 0.9f));
        DrawRectangleLinesEx(textRightRect, 1, textColor);

        DrawPSFText(font,
                    textRightRect.x + padding,
                    textRightRect.y + padding / 2,
                    textRight, spacing, textColor);
    }

    // Малюємо числове значення слайдера у прямокутнику праворуч, нижче textRight
    char valueText[16];
    snprintf(valueText, sizeof(valueText), "%.2f", *value);

    int paddingVal = 6;
    int charCountVal = utf8_strlen(valueText);
    float valTextWidth = charCountVal * (font.width + spacing) - spacing;
    float valBoxWidth = valTextWidth + 2 * paddingVal;
    float valBoxHeight = font.height + 2 * paddingVal;

    Rectangle valueRect = {
        bounds.x + bounds.width + 12,
        bounds.y + bounds.height / 2.0f + 20,
        valBoxWidth,
        valBoxHeight
    };

    DrawRectangleRec(valueRect, baseColor);
    DrawRectangleLinesEx(valueRect, 1, GetContrastingTextColor(baseColor));

    DrawPSFText(font,
                valueRect.x + paddingVal,
                valueRect.y + paddingVal / 2,
                valueText, spacing, GetContrastingTextColor(baseColor));

    // Повертаємо оновлене значення слайдера
    return *value;
}

