// sliders_ex.c

#include "raylib.h"
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

#include "psf_font.h"  // Підключення PSF шрифтів

extern PSF_Font font18;  // Зовнішній шрифт для тексту

// Глобальні змінні-налаштування для відступів (як і в guicheckbox.c)
extern int LineSpacing;  // Відступ між рядками (пікселі)
extern int spacing;      // Відступ між символами (пікселі)

#define SLIDER_KNOB_SIZE 12    // Розмір ручки слайдера (ширина або висота залежно від орієнтації)
#define MAX_SLIDERS 10         // Максимальна кількість слайдерів, які одночасно можуть існувати

// Масив для збереження стану активності кожного слайдера за індексом sliderId
static bool slidersActiveStates[MAX_SLIDERS] = {0};

// Індекс слайдера, який зараз перетягується (-1, якщо перетягування відсутнє)
static int activeDraggingSlider = -1;

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

// Обчислення кількості символів у UTF-8 рядку (не байтів!)
// Ігнорує байти продовження UTF-8 послідовності (0x80 – 0xBF)
/*
static int utf8_strlen(const char* s) {
    int len = 0;
    while (*s) {
        if ((*s & 0xc0) != 0x80) len++;
        s++;
    }
    return len;
} */

// Головна функція слайдера з унікальним індексом sliderId
// sliderId - унікальний індекс слайдера (0..MAX_SLIDERS-1)
// bounds - прямокутник слайдера (позиція і розмір)
// font - шрифт для тексту
// textTop - текст підказки над слайдером (підтримує \n для переносів)
// textRight - текст праворуч від слайдера (підтримує \n для переносів)
// value - вказівник на поточне значення слайдера
// minValue, maxValue - межі значення слайдера
// isVertical - орієнтація слайдера (true - вертикальний, false - горизонтальний)
// baseColor - базовий колір слайдера
float Gui_SliderEx(int sliderId, Rectangle bounds, PSF_Font font, const char *textTop, const char *textRight,
                   float *value, float minValue, float maxValue, bool isVertical, Color baseColor) {
    // Перевірка коректності sliderId
    if (sliderId < 0 || sliderId >= MAX_SLIDERS) return *value;

    // Вказівник на стан активності слайдера за індексом
    bool *isActive = &slidersActiveStates[sliderId];

    // Отримуємо позицію миші
    Vector2 mousePos = GetMousePosition();

    // Обчислюємо нормалізоване значення слайдера (0..1)
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

    // Якщо натиснута ліва кнопка миші, курсор у зоні прилипання, і не перетягується інший слайдер
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && mouseOverSticky && activeDraggingSlider == -1) {
        *isActive = true;  // Активуємо перетягування для цього слайдера
        activeDraggingSlider = sliderId;  // Запам'ятовуємо індекс активного слайдера
    }

    // Якщо відпущена ліва кнопка миші і слайдер активний — завершуємо перетягування
    if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON) && *isActive) {
        *isActive = false;
        activeDraggingSlider = -1;
    }

    // Якщо слайдер активний і це саме він перетягується — оновлюємо значення слайдера
    if (*isActive && activeDraggingSlider == sliderId) {
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
    if (CheckCollisionPointRec(mousePos, bounds)) sliderBgColor = Fade(sliderBgColor, 0.8f);  // Трохи яскравіший при наведенні

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

    // --- МАЛЮЄМО ПІДКАЗКУ textTop (багаторядковий текст з переносами по '\n', показ лише при фокусі миші) ---

    if (CheckCollisionPointRec(mousePos, bounds) && textTop && textTop[0] != '\0') {
        int padding = 6;  // Відступи всередині рамки

        // Розбиваємо textTop на рядки
        const char* lines[10];
        int lineCount = 0;

        char tempText[256];
        strncpy(tempText, textTop, sizeof(tempText) - 1);
        tempText[sizeof(tempText) - 1] = '\0';

        char* line = strtok(tempText, "\n");
        while (line != NULL && lineCount < 10) {
            lines[lineCount++] = line;
            line = strtok(NULL, "\n");
        }

        // Обчислюємо максимальну ширину серед рядків
        float maxWidth = 0;
        for (int i = 0; i < lineCount; i++) {
            int charCount = utf8_strlen(lines[i]);
            float lineWidth = charCount * (font.width + spacing) - spacing;
            if (lineWidth > maxWidth) maxWidth = lineWidth;
        }

        float lineHeight = (float)font.height;
        float totalHeight = lineCount * lineHeight + (lineCount - 1) * LineSpacing + 2 * padding;

        // Прямокутник підказки над слайдером
        Rectangle tooltipRect = {
            bounds.x + bounds.width / 2.0f - (maxWidth + 2 * padding) / 2.0f,
            bounds.y - totalHeight - 8, // відступ зверху
            maxWidth + 2 * padding,
            totalHeight
        };

        DrawRectangleRec(tooltipRect, Fade(borderColor, 0.9f));
        DrawRectangleLinesEx(tooltipRect, 1, textColor);

        for (int i = 0; i < lineCount; i++) {
            DrawPSFText(font,
                        (int)(tooltipRect.x + padding),
                        (int)(tooltipRect.y + padding / 2 + i * (lineHeight + LineSpacing)),
                        lines[i], spacing, textColor);
        }
    }

    // --- МАЛЮЄМО ПІДКАЗКУ textRight (багаторядковий текст з переносами по '\n', показ лише при фокусі миші) ---

    if (CheckCollisionPointRec(mousePos, bounds) && textRight && textRight[0] != '\0') {
        int padding = 6;

        const char* lines[10];
        int lineCount = 0;

        char tempText[256];
        strncpy(tempText, textRight, sizeof(tempText) - 1);
        tempText[sizeof(tempText) - 1] = '\0';

        char* line = strtok(tempText, "\n");
        while (line != NULL && lineCount < 10) {
            lines[lineCount++] = line;
            line = strtok(NULL, "\n");
        }

        float maxWidth = 0;
        for (int i = 0; i < lineCount; i++) {
            int charCount = utf8_strlen(lines[i]);
            float lineWidth = charCount * (font.width + spacing) - spacing;
            if (lineWidth > maxWidth) maxWidth = lineWidth;
        }

        float lineHeight = (float)font.height;
        float totalHeight = lineCount * lineHeight + (lineCount - 1) * LineSpacing + 2 * padding;

        Rectangle textRightRect = {
            bounds.x + bounds.width + 12,
            bounds.y + bounds.height / 2.0f - totalHeight / 2.0f,
            maxWidth + 2 * padding,
            totalHeight
        };

        DrawRectangleRec(textRightRect, Fade(borderColor, 0.9f));
        DrawRectangleLinesEx(textRightRect, 1, textColor);

        for (int i = 0; i < lineCount; i++) {
            DrawPSFText(font,
                        (int)(textRightRect.x + padding),
                        (int)(textRightRect.y + padding / 2 + i * (lineHeight + LineSpacing)),
                        lines[i], spacing, textColor);
        }
    }

    // --- МАЛЮЄМО ЧИСЛОВЕ ЗНАЧЕННЯ слайдера (Показ тільки при фокусі над слайдером) ---

    if (CheckCollisionPointRec(mousePos, bounds)) {
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
    }

    // Повертаємо оновлене значення слайдера
    return *value;
}

