// sliders.c

#include "raylib.h"
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

#include "psf_font.h"  // Підключення PSF шрифтів
extern PSF_Font font18;  // Зовнішній шрифт для тексту

// Глобальні налаштування відступів (як в рендері guicheckbox.c)
extern int LineSpacing;  // Відступ між рядками (пікселі)
extern int spacing;      // Відступ між символами (пікселі)

#define SLIDER_KNOB_SIZE 12    // Розмір ручки слайдера (ширина або висота залежно від орієнтації)
#define MAX_SLIDERS 10         // Максимальна кількість слайдерів одночасно

// Структура для збереження даних слайдера
typedef struct {
    Rectangle bounds;  // Положення і розмір слайдера
    bool isActive;     // Чи активний слайдер (перетягується)
    bool used;         // Чи використовується цей слот
} SliderState;

// Масив станів слайдерів
static SliderState slidersState[MAX_SLIDERS] = {0};

// Індекс слайдера, який перетягується (-1, якщо відсутній)
static int activeDraggingSlider = -1;

// Пошук або ініціалізація слайдера за bounds
static bool* GetSliderActiveState(Rectangle bounds) {
    for (int i = 0; i < MAX_SLIDERS; i++) {
        if (slidersState[i].used) {
            // Якщо знайдено слайдер із такими ж bounds - повертаємо вказівник на його isActive
            if (memcmp(&slidersState[i].bounds, &bounds, sizeof(Rectangle)) == 0) {
                return &slidersState[i].isActive;
            }
        } else {
            // Якщо слот вільний, ініціалізуємо слайдер і повертаємо вказівник на isActive
            slidersState[i].bounds = bounds;
            slidersState[i].isActive = false;
            slidersState[i].used = true;
            return &slidersState[i].isActive;
        }
    }
    // Якщо всі слоти зайняті, повертаємо NULL
    return NULL;
}

// Функція обчислення яскравості кольору (luminance) для вибору контрастного тексту
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

// Підрахунок кількості символів UTF-8 (а не байтів)
/*
static int utf8_strlen(const char* s) {
    int len = 0;
    while (*s) {
        if ((*s & 0xC0) != 0x80) len++;
        s++;
    }
    return len;
} */

// Функція розбиття рядка по '\n' на лінії (в межах maxLines), заповнює масив рядків
// Повертає кількість рядків
static int SplitTextLines(char *text, const char **lines, int maxLines) {
    int count = 0;
    char *line = strtok(text, "\n");
    while (line != NULL && count < maxLines) {
        lines[count++] = line;
        line = strtok(NULL, "\n");
    }
    return count;
}

// Головна функція слайдера
// bounds - прямокутник слайдера
// font - шрифт для тексту
// textTop - багаторядковий текст (підказка над слайдером, підтримує \n)
// textRight - багаторядковий текст праворуч (підказка, підтримує \n)
// value - вказівник на значення слайдера
// minValue, maxValue - межі значення слайдера
// isVertical - true для вертикального слайдера, false - горизонтального
// baseColor - колір слайдера
float Gui_Slider(Rectangle bounds, PSF_Font font, const char *textTop, const char *textRight,
                 float *value, float minValue, float maxValue, bool isVertical, Color baseColor) {
    // Отримуємо вказівник на активність слайдера
    bool *isActive = GetSliderActiveState(bounds);
    if (!isActive) return *value;  // Якщо немає слотів — повертаємо поточне значення

    Vector2 mousePos = GetMousePosition();  // Позиція миші
    bool mouseOver = CheckCollisionPointRec(mousePos, bounds); // Чи курсор над слайдером

    // Нормалізоване значення в [0..1]
    float normValue = (*value - minValue) / (maxValue - minValue);
    if (normValue < 0) normValue = 0;
    if (normValue > 1) normValue = 1;

    // Обробка початку перетягування миші по слайдеру
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && mouseOver && activeDraggingSlider == -1) {
        *isActive = true;
        // Знаходимо індекс слайдера в масиві
        for (int i = 0; i < MAX_SLIDERS; i++) {
            if (slidersState[i].used && memcmp(&slidersState[i].bounds, &bounds, sizeof(Rectangle)) == 0) {
                activeDraggingSlider = i;
                break;
            }
        }
    }
    // Завершення перетягування, якщо кнопка відпущена
    if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON) && *isActive) {
        *isActive = false;
        activeDraggingSlider = -1;
    }

    // Якщо слайдер перетягується, оновлюємо значення
    if (*isActive && activeDraggingSlider != -1) {
        if (isVertical) {
            // Для вертикального слайдера інвертуємо, щоб верх = maxValue
            normValue = 1.0f - (mousePos.y - bounds.y) / bounds.height;
        } else {
            normValue = (mousePos.x - bounds.x) / bounds.width;
        }
        if (normValue < 0) normValue = 0;
        if (normValue > 1) normValue = 1;

        *value = minValue + normValue * (maxValue - minValue);
    }

    // Підбір кольору фону слайдера з урахуванням активності і наведення миші
    Color sliderBgColor = baseColor;
    if (!(*isActive)) sliderBgColor = Fade(sliderBgColor, 0.4f);  // Зменшуємо яскравість, якщо неактивний
    if (mouseOver) sliderBgColor = Fade(baseColor, 0.8f);        // Трохи підсвічуємо при наведенні

    // Колір рамки та тексту контрастний до фону
    Color borderColor = GetContrastingTextColor(sliderBgColor);
    Color textColor = GetContrastingTextColor(borderColor);
    textColor.a = 255;  // Повна непрозорість тексту

    // Малюємо фон слайдера
    DrawRectangleRec(bounds, sliderBgColor);

    // Малюємо подвійний контур слайдера
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

    // --- Малювання textTop (підказка над слайдером) при фокусі (коли миша над слайдером) ---

    if (mouseOver && textTop && textTop[0] != '\0') {
        int padding = 6;  // Внутрішні відступи в підказці

        // Копіюємо тект у тимчасовий буфер для strtok (розбиття по '\n')
        char tempText[256];
        strncpy(tempText, textTop, sizeof(tempText)-1);
        tempText[sizeof(tempText)-1] = '\0';

        // Масив вказівників на рядки
        const char *lines[10];
        int lineCount = SplitTextLines(tempText, lines, 10);

        // Обчислюємо максимальну ширину рядків
        float maxWidth = 0;
        for (int i = 0; i < lineCount; i++) {
            int charCount = utf8_strlen(lines[i]);
            float lineWidth = charCount * (font.width + spacing) - spacing;
            if (lineWidth > maxWidth) maxWidth = lineWidth;
        }

        float lineHeight = (float)font.height;
        float totalHeight = lineCount * lineHeight + (lineCount - 1) * LineSpacing + 2 * padding;

        // прямокутник для підказки над слайдером (по центру)
        Rectangle tooltipRect = {
            bounds.x + bounds.width / 2.0f - (maxWidth + 2 * padding) / 2.0f,
            bounds.y - totalHeight - 8, // невеликий відступ зверху
            maxWidth + 2 * padding,
            totalHeight
        };

        DrawRectangleRec(tooltipRect, Fade(borderColor, 0.9f));
        DrawRectangleLinesEx(tooltipRect, 1, textColor);

        // Малюємо кожен рядок тексту
        for (int i = 0; i < lineCount; i++) {
            DrawPSFText(font,
                        (int)(tooltipRect.x + padding),
                        (int)(tooltipRect.y + padding / 2 + i * (lineHeight + LineSpacing)),
                        lines[i], spacing, textColor);
        }
    }

    // --- Малювання textRight (підказка праворуч від слайдера) при фокусі ---

    if (mouseOver && textRight && textRight[0] != '\0') {
        int padding = 6;

        // Копія тексту для strtok
        char tempText[256];
        strncpy(tempText, textRight, sizeof(tempText)-1);
        tempText[sizeof(tempText)-1] = '\0';

        const char *lines[10];
        int lineCount = SplitTextLines(tempText, lines, 10);

        // Визначення макс ширини серед рядків
        float maxWidth = 0;
        for (int i = 0; i < lineCount; i++) {
            int charCount = utf8_strlen(lines[i]);
            float lineWidth = charCount * (font.width + spacing) - spacing;
            if (lineWidth > maxWidth) maxWidth = lineWidth;
        }

        float lineHeight = (float)font.height;
        float totalHeight = lineCount * lineHeight + (lineCount - 1) * LineSpacing + 2 * padding;

        // Рамка для підказки праворуч, відцентрованої вертикально
        Rectangle textRightRect = {
            bounds.x + bounds.width + 12,
            bounds.y + bounds.height / 2.0f - totalHeight / 2.0f,
            maxWidth + 2 * padding,
            totalHeight
        };

        DrawRectangleRec(textRightRect, Fade(borderColor, 0.9f));
        DrawRectangleLinesEx(textRightRect, 1, textColor);

        // Малюємо кожен рядок
        for (int i = 0; i < lineCount; i++) {
            DrawPSFText(font,
                        (int)(textRightRect.x + padding),
                        (int)(textRightRect.y + padding / 2 + i * (lineHeight + LineSpacing)),
                        lines[i], spacing, textColor);
        }
    }

    // --- Малювання числового значення слайдера лише коли миша наведенна (фокус над слайдером) ---

    if (mouseOver) {
        char valueText[16];
        snprintf(valueText, sizeof(valueText), "%.2f", *value);

        int paddingVal = 6;
        int charCountVal = utf8_strlen(valueText);
        float valTextWidth = charCountVal * (font.width + spacing) - spacing;
        float valBoxWidth = valTextWidth + 2 * paddingVal;
        float valBoxHeight = font.height + 2 * paddingVal;

        // прямокутник розташований правіше, нижче textRight
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

