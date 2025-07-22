// sliders_ex.c

#include "sliders_ex.h"
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

#include "psf_font.h"  // Ваш парсер PSF шрифтів
extern int spacing;   // Відступ між символами у тексті

// Масив слайдерів
static SliderEx sliders[MAX_SLIDERS];
static int slidersCount = 0;

// Стек активних слайдерів (LIFO) - зберігає індекси слайдерів, які зараз перетягуються
typedef struct {
    int indices[MAX_SLIDERS];  // Індекси активних слайдерів
    int top;                   // Індекс вершини стеку (-1, якщо порожній)
} ActiveSliderStack;

static ActiveSliderStack activeStack = {.top = -1};

// Додає слайдер у стек, якщо він уже є - видаляє і додає наверх (щоб не було дублікатів)
static void StackPush(int idx) {
    // Шукаємо слайдер у стеку
    for (int i = 0; i <= activeStack.top; i++) {
        if (activeStack.indices[i] == idx) {
            // Видаляємо зі стеку, щоб потім додати наверх
            for (int j = i; j < activeStack.top; j++) {
                activeStack.indices[j] = activeStack.indices[j + 1];
            }
            activeStack.top--;
            break;
        }
    }
    // Додаємо наверх стеку
    if (activeStack.top < MAX_SLIDERS - 1) {
        activeStack.indices[++activeStack.top] = idx;
    }
}

// Видаляє слайдер зі стеку
static void StackPop(int idx) {
    for (int i = 0; i <= activeStack.top; i++) {
        if (activeStack.indices[i] == idx) {
            for (int j = i; j < activeStack.top; j++) {
                activeStack.indices[j] = activeStack.indices[j + 1];
            }
            activeStack.top--;
            break;
        }
    }
}

// Повертає індекс слайдера, який зараз на вершині стеку (останній активний)
static int StackTop() {
    return (activeStack.top >= 0) ? activeStack.indices[activeStack.top] : -1;
}

// Ініціалізація слайдерів (приклад)
void InitSliders(void) {
    slidersCount = 4;  // Кількість слайдерів

    // Вертикальні слайдери
    sliders[0] = (SliderEx){{75, 50, 10, 450}, 50.0f, 0.0f, 100.0f, true, RED, false};
    sliders[1] = (SliderEx){{75, 50, 10, 450}, 50.0f, 0.0f, 100.0f, true, ORANGE, false};
    sliders[2] = (SliderEx){{75, 50, 10, 450}, 50.0f, 0.0f, 100.0f, true, GREEN, false};

    // Горизонтальний слайдер
    sliders[3] = (SliderEx){{200, 250, 300, 20}, 50.0f, 0.0f, 100.0f, false, DARKGRAY, false};

    activeStack.top = -1;  // Очищаємо стек активних
}

// Перевірка, чи знаходиться курсор миші в зоні прилипання ручки слайдера
static bool IsMouseNearSliderKnob(Vector2 mousePos, SliderEx *slider) {
    float normValue = (slider->value - slider->minValue) / (slider->maxValue - slider->minValue);
    Rectangle knobRect;
    if (slider->isVertical) {
        float knobY = slider->bounds.y + (1.0f - normValue) * slider->bounds.height;
        knobRect = (Rectangle){ slider->bounds.x, knobY - SLIDER_KNOB_SIZE / 2, slider->bounds.width, SLIDER_KNOB_SIZE };
    } else {
        float knobX = slider->bounds.x + normValue * slider->bounds.width;
        knobRect = (Rectangle){ knobX - SLIDER_KNOB_SIZE / 2, slider->bounds.y, SLIDER_KNOB_SIZE, slider->bounds.height };
    }
    const int stickyMargin = 10;
    if (slider->isVertical) {
        knobRect.y -= stickyMargin;
        knobRect.height += 2 * stickyMargin;
    } else {
        knobRect.x -= stickyMargin;
        knobRect.width += 2 * stickyMargin;
    }
    return CheckCollisionPointRec(mousePos, knobRect);
}

// Оновлення значення слайдера по позиції миші
static void UpdateSliderValue(SliderEx *slider, Vector2 mousePos) {
    float normValue;
    if (slider->isVertical) {
        normValue = 1.0f - (mousePos.y - slider->bounds.y) / slider->bounds.height;
    } else {
        normValue = (mousePos.x - slider->bounds.x) / slider->bounds.width;
    }
    if (normValue < 0) normValue = 0;
    if (normValue > 1) normValue = 1;
    slider->value = slider->minValue + normValue * (slider->maxValue - slider->minValue);
}

// Оновлення слайдерів: обробка миші та перетягування
void UpdateSliders(Vector2 mousePos, bool mousePressed, bool mouseDown, bool mouseReleased) {
    // Обробка початку перетягування слайдера
    if (mousePressed) {
        // Перевіряємо слайдери у зворотному порядку (щоб верхній мав пріоритет)
        for (int i = slidersCount - 1; i >= 0; i--) {
            if (IsMouseNearSliderKnob(mousePos, &sliders[i])) {
                if (!sliders[i].isActive) {
                    StackPush(i);          // Додаємо слайдер у стек активних
                    sliders[i].isActive = true;

                    // Переміщаємо слайдер у кінець масиву (щоб малювався зверху)
                    SliderEx temp = sliders[i];
                    for (int j = i; j < slidersCount - 1; j++) {
                        sliders[j] = sliders[j + 1];
                    }
                    sliders[slidersCount - 1] = temp;

                    // Оновлюємо індекс у стеку (верхній елемент)
                    activeStack.indices[activeStack.top] = slidersCount - 1;
                }
                break; // Захопили один слайдер — виходимо
            }
        }
    }

    // Обробка перетягування: оновлюємо значення тільки для верхнього слайдера у стеку
    if (mouseDown && activeStack.top >= 0) {
        int activeIdx = activeStack.indices[activeStack.top];
        if (activeIdx >= 0 && activeIdx < slidersCount) {
            UpdateSliderValue(&sliders[activeIdx], mousePos);
        }
    }

    // Обробка завершення перетягування (відпускання кнопки миші)
    if (mouseReleased && activeStack.top >= 0) {
        int activeIdx = activeStack.indices[activeStack.top];
        if (activeIdx >= 0 && activeIdx < slidersCount) {
            sliders[activeIdx].isActive = false;
            StackPop(activeIdx);
        }
    }
}

// Малювання фону, рамки та тексту слайдера
static void DrawSliderBackground(SliderEx *slider, PSF_Font font, const char *textTop, const char *textRight) {
    Color bgColor = slider->baseColor;

    // Зменшуємо яскравість, якщо слайдер не активний
    if (!slider->isActive) bgColor = Fade(bgColor, 0.4f);

    // Якщо курсор миші над слайдером — робимо фон яскравішим
    if (CheckCollisionPointRec(GetMousePosition(), slider->bounds)) bgColor = Fade(bgColor, 0.8f);

    // Визначаємо контрастний колір для рамки і тексту
    float luminance = 0.2126f * bgColor.r / 255.0f + 0.7152f * bgColor.g / 255.0f + 0.0722f * bgColor.b / 255.0f;
    Color borderColor = (luminance > 0.5f) ? BLACK : WHITE;
    Color textColor = borderColor;
    textColor.a = 255;

    // Малюємо фон слайдера
    DrawRectangleRec(slider->bounds, Fade(bgColor, 0.25f));
    DrawRectangleLinesEx(slider->bounds, 1, borderColor);

    Vector2 mousePos = GetMousePosition();

    // Малюємо текст зверху слайдера, якщо він заданий і курсор над слайдером
    if (textTop && textTop[0] != '\0' && CheckCollisionPointRec(mousePos, slider->bounds)) {
        int padding = 6;
        int charCount = 0;
        const char *p = textTop;
        while (*p) { if ((*p & 0xc0) != 0x80) charCount++; p++; }
        float textWidth = charCount * (font.width + spacing) - spacing;
        float boxWidth = textWidth + 2 * padding;
        float boxHeight = font.height + 2 * padding;

        Rectangle tooltipRect = {
            slider->bounds.x + slider->bounds.width / 2.0f - boxWidth / 2.0f,
            slider->bounds.y - boxHeight - 8,
            boxWidth,
            boxHeight
        };

        DrawRectangleRec(tooltipRect, Fade(borderColor, 0.9f));
        DrawRectangleLinesEx(tooltipRect, 1, textColor);

        DrawPSFText(font, tooltipRect.x + padding, tooltipRect.y + padding / 2, textTop, spacing, textColor);
    }

    // Малюємо текст праворуч від слайдера, якщо заданий
    if (textRight && textRight[0] != '\0') {
        int padding = 6;
        int charCount = 0;
        const char *p = textRight;
        while (*p) { if ((*p & 0xc0) != 0x80) charCount++; p++; }
        float textWidth = charCount * (font.width + spacing) - spacing;
        float boxWidth = textWidth + 2 * padding;
        float boxHeight = font.height + 2 * padding;

        Rectangle textRightRect = {
            slider->bounds.x + slider->bounds.width + 12,
            slider->bounds.y + slider->bounds.height / 2.0f - boxHeight / 2.0f,
            boxWidth,
            boxHeight
        };

        DrawRectangleRec(textRightRect, Fade(borderColor, 0.9f));
        DrawRectangleLinesEx(textRightRect, 1, textColor);

        DrawPSFText(font, textRightRect.x + padding, textRightRect.y + padding / 2, textRight, spacing, textColor);
    }

    // Малюємо числове значення слайдера праворуч, нижче textRight
    char valueText[16];
    snprintf(valueText, sizeof(valueText), "%.2f", slider->value);

    int paddingVal = 6;
    int charCountVal = 0;
    const char *pv = valueText;
    while (*pv) { if ((*pv & 0xc0) != 0x80) charCountVal++; pv++; }
    float valTextWidth = charCountVal * (font.width + spacing) - spacing;
    float valBoxWidth = valTextWidth + 2 * paddingVal;
    float valBoxHeight = font.height + 2 * paddingVal;

    Rectangle valueRect = {
        slider->bounds.x + slider->bounds.width + 12,
        slider->bounds.y + slider->bounds.height / 2.0f + 20,
        valBoxWidth,
        valBoxHeight
    };

    DrawRectangleRec(valueRect, slider->baseColor);
    DrawRectangleLinesEx(valueRect, 1, borderColor);

    DrawPSFText(font, valueRect.x + paddingVal, valueRect.y + paddingVal / 2, valueText, spacing, borderColor);
}

// Малювання всіх слайдерів у порядку масиву (останній малюється зверху)
void DrawSliders(PSF_Font font) {
    for (int i = 0; i < slidersCount; i++) {
        DrawSliderBackground(&sliders[i], font, (i < 3) ? "Vertical" : "Horizontal", NULL);

        // Малюємо ручку слайдера
        SliderEx *slider = &sliders[i];
        float normValue = (slider->value - slider->minValue) / (slider->maxValue - slider->minValue);
        Color knobColor = slider->baseColor;

        if (slider->isVertical) {
            float knobY = slider->bounds.y + (1.0f - normValue) * slider->bounds.height;
            DrawRectangle(slider->bounds.x, knobY - SLIDER_KNOB_SIZE / 2, slider->bounds.width, SLIDER_KNOB_SIZE, knobColor);
        } else {
            float knobX = slider->bounds.x + normValue * slider->bounds.width;
            DrawRectangle(knobX - SLIDER_KNOB_SIZE / 2, slider->bounds.y, SLIDER_KNOB_SIZE, slider->bounds.height, knobColor);
        }
    }
}

// Функція для отримання значення слайдера за індексом (оновлення відбувається у UpdateSliders)
float Gui_SliderEx(int sliderId, PSF_Font font, const char *textTop, const char *textRight) {
    if (sliderId < 0 || sliderId >= slidersCount) return 0.0f;
    // Просто повертаємо значення слайдера
    return sliders[sliderId].value;
}

