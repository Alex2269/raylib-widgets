// slider_widget.c
// Віджет слайдерів з підтримкою множинних слайдерів,
// з активною ручкою, підказками і контрастним малюванням.

// Підключення необхідних заголовків
#include "slider_widget.h"
#include <stdio.h>
#include <string.h>
#include <math.h>

// Константи для обмежень і параметрів слайдера
#define MAX_SLIDERS 16       // Максимальна кількість слайдерів
#define SLIDER_KNOB_SIZE 12  // Розмір ручки слайдера (ширина або висота в пікселях)
#define CAPTURE_RADIUS 10    // Радіус області "захоплення" мишею навколо ручки

// Структура описує параметри одного слайдера
typedef struct {
    Rectangle bounds;        // Область, у якій малюється слайдер на екрані
    float value;             // Поточне значення слайдера
    float minValue;          // Мінімальне можливе значення слайдера
    float maxValue;          // Максимальне можливе значення слайдера
    bool isVertical;         // Орієнтація: true - вертикальний, false - горизонтальний
    Color baseColor;         // Базовий колір елемента слайдера (фону, ручки)
    bool isActive;           // Чи активний слайдер (чи ручка захоплена мишею)
    bool used;               // Чи цей слот слайдера зайнятий
    const char* textTop;     // Текстова підказка над слайдером (може бути NULL)
    const char* textRight;   // Підказка праворуч від слайдера (може бути NULL)
} SliderEx;

// Масив для збереження інформації про всі слайдери
static SliderEx sliders[MAX_SLIDERS] = {0};
static int slidersCount = 0;          // Кількість зареєстрованих слайдерів
static int activeSliderIndex = -1;    // Індекс активного слайдера (-1, якщо відсутній)

// Функція для малювання тексту (зовнішня, реалізується поза цим файлом)
extern void DrawPSFText(PSF_Font font, int x, int y, const char* text, int spacing, Color color);

// Глобальні змінні для керування відступами в тексті
extern int LineSpacing;  // Відступ між рядками (пікселі)
extern int spacing;      // Відступ між символами (пікселі)

// Функція підрахунку кількості UTF-8 символів у рядку (не байтів!)
// Ігнорує байти продовження (ASCII extension)
/*
static int utf8_strlen(const char* s)
{
    int len = 0;
    while (*s) {
        if ((*s & 0xC0) != 0x80) len++;  // Рахуємо тільки стартові байти символів UTF-8
        s++;
    }
    return len;
} */

// Змінює насиченість кольору (яскравість) у HSV моделі, для підсвічування
static Color ChangeSaturation(Color c, float saturationScale) {
    float r = c.r / 255.0f;
    float g = c.g / 255.0f;
    float b = c.b / 255.0f;

    float cMax = fmaxf(r, fmaxf(g, b));
    float cMin = fminf(r, fminf(g, b));
    float delta = cMax - cMin;

    float h = 0.0f;
    if (delta > 0) {
        if (cMax == r) h = fmodf((g - b) / delta, 6.0f);
        else if (cMax == g) h = (b - r) / delta + 2.0f;
        else h = (r - g) / delta + 4.0f;
        h *= 60.0f;
        if (h < 0) h += 360.0f;
    }

    float s = (cMax == 0) ? 0 : delta / cMax;
    float v = cMax;

    s *= saturationScale;
    if (s > 1.0f) s = 1.0f;

    float cVal = v * s;
    float x = cVal * (1 - fabsf(fmodf(h / 60.0f, 2) - 1));
    float m = v - cVal;

    Color newColor = {0};
    if (h >= 0 && h < 60) {
        newColor.r = (cVal + m) * 255;
        newColor.g = (x + m) * 255;
        newColor.b = (0 + m) * 255;
    } else if (h >= 60 && h < 120) {
        newColor.r = (x + m) * 255;
        newColor.g = (cVal + m) * 255;
        newColor.b = (0 + m) * 255;
    } else if (h >= 120 && h < 180) {
        newColor.r = (0 + m) * 255;
        newColor.g = (cVal + m) * 255;
        newColor.b = (x + m) * 255;
    } else if (h >= 180 && h < 240) {
        newColor.r = (0 + m) * 255;
        newColor.g = (x + m) * 255;
        newColor.b = (cVal + m) * 255;
    } else if (h >= 240 && h < 300) {
        newColor.r = (x + m) * 255;
        newColor.g = (0 + m) * 255;
        newColor.b = (cVal + m) * 255;
    } else if (h >= 300 && h < 360) {
        newColor.r = (cVal + m) * 255;
        newColor.g = (0 + m) * 255;
        newColor.b = (x + m) * 255;
    }
    newColor.a = c.a; // Альфа канал не змінюємо
    return newColor;
}

// Перевіряє, чи знаходиться миша у зоні "захоплення" ручки слайдера
static bool IsMouseNearKnob(Vector2 mousePos, SliderEx *slider) {
    if (!slider->used) return false;

    // Обчислюємо нормалізоване значення (0..1)
    float normValue = (slider->value - slider->minValue) / (slider->maxValue - slider->minValue);
    float knobX, knobY;

    // Обчислюємо координати ручки в залежності від орієнтації слайдера
    if (slider->isVertical) {
        knobX = slider->bounds.x + slider->bounds.width / 2.0f;
        knobY = slider->bounds.y + (1.0f - normValue) * slider->bounds.height;
    } else {
        knobX = slider->bounds.x + normValue * slider->bounds.width;
        knobY = slider->bounds.y + slider->bounds.height / 2.0f;
    }

    float dx = mousePos.x - knobX;
    float dy = mousePos.y - knobY;
    float distSq = dx * dx + dy * dy;

    // Повертаємо true, якщо миша в межах радіуса захоплення ручки
    return (distSq <= CAPTURE_RADIUS * CAPTURE_RADIUS);
}

// Оновлює значення слайдера відповідно до позиції миші
static void UpdateValueFromMouse(SliderEx *slider, Vector2 mousePos) {
    float normValue;
    if (slider->isVertical) {
        normValue = 1.0f - (mousePos.y - slider->bounds.y) / slider->bounds.height;
    } else {
        normValue = (mousePos.x - slider->bounds.x) / slider->bounds.width;
    }
    normValue = fmaxf(0.0f, fminf(1.0f, normValue)); // Обрізаємо значення в діапазон [0..1]
    slider->value = slider->minValue + normValue * (slider->maxValue - slider->minValue);
}

// Обчислює контрастний колір - чорний або білий - для тексту на фоні
static Color GetContrastingColor(Color c) {
    float luminance = 0.2126f * c.r / 255.0f + 0.7152f * c.g / 255.0f + 0.0722f * c.b / 255.0f;
    return (luminance > 0.5f) ? BLACK : WHITE;
}

// Функція реєстрації слайдера (один раз на кадр)
void RegisterSlider(int sliderIndex, Rectangle bounds, float value, float minValue, float maxValue, bool isVertical, Color baseColor, const char* textTop, const char* textRight) {
    if (sliderIndex < 0 || sliderIndex >= MAX_SLIDERS) return;

    // Якщо слот не зайнятий, ініціалізуємо параметри
    if (!sliders[sliderIndex].used) {
        sliders[sliderIndex].bounds = bounds;
        sliders[sliderIndex].minValue = minValue;
        sliders[sliderIndex].maxValue = maxValue;
        sliders[sliderIndex].isVertical = isVertical;
        sliders[sliderIndex].baseColor = baseColor;
        sliders[sliderIndex].used = true;
        sliders[sliderIndex].textTop = textTop;
        sliders[sliderIndex].textRight = textRight;
        sliders[sliderIndex].value = value;

        if (sliderIndex >= slidersCount)
            slidersCount = sliderIndex + 1;
    }
}

// Головна функція оновлення та малювання слайдерів
// Викликається кожен кадр, оновлює значення і малює всі елементи
void UpdateSlidersAndDraw(PSF_Font font, int localSpacing) {
    Vector2 mousePos = GetMousePosition();

    // Обробляємо натискання миші - виявляємо активний слайдер
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        if (activeSliderIndex == -1) {
            // Перебираємо слайдери у зворотньому порядку
            // (щоб активувати слайдер, намальований зверху)
            for (int i = slidersCount - 1; i >= 0; i--) {
                if (IsMouseNearKnob(mousePos, &sliders[i])) {
                    activeSliderIndex = i;
                    sliders[i].isActive = true;
                    // Переміщуємо активний слайдер в кінець масиву для правильного малювання
                    if (activeSliderIndex != slidersCount - 1) {
                        SliderEx temp = sliders[activeSliderIndex];
                        for (int j = activeSliderIndex; j < slidersCount - 1; j++) {
                            sliders[j] = sliders[j + 1];
                        }
                        sliders[slidersCount - 1] = temp;
                        activeSliderIndex = slidersCount - 1;
                    }
                    break;
                }
            }
        }
    }
    // Обробляємо відпускання кнопки миші
    else if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
        if (activeSliderIndex != -1) {
            sliders[activeSliderIndex].isActive = false;
            activeSliderIndex = -1;
        }
    }

    // Якщо кнопка миші тримається та є активний слайдер, оновлюємо його значення
    if (activeSliderIndex != -1 && IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
        UpdateValueFromMouse(&sliders[activeSliderIndex], mousePos);
    }

    // Малюємо всі слайдери (фони, рамки, ручки)
    for (int i = 0; i < slidersCount; i++) {
        SliderEx *slider = &sliders[i];
        float normVal = (slider->value - slider->minValue) / (slider->maxValue - slider->minValue);

        // Малюємо напівпрозорий фон і контрастну рамку слайдера
        DrawRectangleRec(slider->bounds, Fade(slider->baseColor, 0.20f));
        DrawRectangleLinesEx(slider->bounds, 1, GetContrastingColor(slider->baseColor));

        // Визначаємо колір ручки: активний, наведений чи базовий
        Color knobColor = slider->baseColor;
        if (slider->isActive)
            knobColor = ChangeSaturation(slider->baseColor, 1.5f);
        else if (IsMouseNearKnob(mousePos, slider))
            knobColor = ChangeSaturation(slider->baseColor, 1.2f);

        // Малюємо ручку (вертикальна або горизонтальна)
        if (slider->isVertical) {
            float knobY = slider->bounds.y + (1.0f - normVal) * slider->bounds.height;
            DrawRectangle(slider->bounds.x, knobY - SLIDER_KNOB_SIZE / 2, slider->bounds.width, SLIDER_KNOB_SIZE, knobColor);
        } else {
            float knobX = slider->bounds.x + normVal * slider->bounds.width;
            DrawRectangle(knobX - SLIDER_KNOB_SIZE / 2, slider->bounds.y, SLIDER_KNOB_SIZE, slider->bounds.height, knobColor);
        }
    }

    // Малюємо підказки та числові значення для активного або наведеного слайдера
    for (int i = slidersCount - 1; i >= 0; i--) {
        SliderEx* slider = &sliders[i];
        if (slider->isActive || IsMouseNearKnob(mousePos, slider)) {
            int padding = 6; // Відступи для внутрішніх відстаней тексту

            // --- Підказка над слайдером (підтримка багаторядковості) ---
            if (slider->textTop && slider->textTop[0] != '\0') {
                const char* lines[10]; // Максимум 10 рядків
                int lineCount = 0;

                // Копіюємо текст у тимчасовий буфер для розбиття
                char tempText[256];
                strncpy(tempText, slider->textTop, sizeof(tempText) - 1);
                tempText[sizeof(tempText) - 1] = '\0';

                // Розбиваємо текст на рядки по символу '\n'
                char* token = strtok(tempText, "\n");
                while (token != NULL && lineCount < 10) {
                    lines[lineCount++] = token;
                    token = strtok(NULL, "\n");
                }

                // Визначаємо максимальну ширину серед рядків
                float maxWidth = 0;
                for (int li = 0; li < lineCount; li++) {
                    int charCount = utf8_strlen(lines[li]);
                    float lineWidth = charCount * (font.width + localSpacing) - localSpacing;
                    if (lineWidth > maxWidth) maxWidth = lineWidth;
                }

                // Визначаємо висоту окремого рядка та загальну висоту блока
                float lineHeight = (float)font.height;
                float totalHeight = lineCount * lineHeight + (lineCount - 1) * LineSpacing + 2 * padding;

                // Позиція та розмір прямокутника фону підказки
                Rectangle tooltipRect = {
                    slider->bounds.x + slider->bounds.width / 2.0f - (maxWidth + 2 * padding) / 2.0f,
                    slider->bounds.y - totalHeight - 8, // Відступ зверху
                    maxWidth + 2 * padding,
                    totalHeight
                };

                DrawRectangleRec(tooltipRect, Fade(BLACK, 0.8f));
                DrawRectangleLinesEx(tooltipRect, 1, WHITE);

                // Малюємо кожен рядок тексту з вертикальними відступами
                for (int li = 0; li < lineCount; li++) {
                    DrawPSFText(font,
                                (int)(tooltipRect.x + padding),
                                (int)(tooltipRect.y + padding / 2 + li * (lineHeight + LineSpacing)),
                                lines[li], localSpacing, WHITE);
                }
            }

            // --- Підказка праворуч від слайдера (також багаторядкова) ---
            if (slider->textRight && slider->textRight[0] != '\0') {
                const char* lines[10];
                int lineCount = 0;

                char tempText[256];
                strncpy(tempText, slider->textRight, sizeof(tempText) - 1);
                tempText[sizeof(tempText) - 1] = '\0';

                char* token = strtok(tempText, "\n");
                while (token != NULL && lineCount < 10) {
                    lines[lineCount++] = token;
                    token = strtok(NULL, "\n");
                }

                float maxWidth = 0;
                for (int li = 0; li < lineCount; li++) {
                    int charCount = utf8_strlen(lines[li]);
                    float lineWidth = charCount * (font.width + localSpacing) - localSpacing;
                    if (lineWidth > maxWidth) maxWidth = lineWidth;
                }

                float lineHeight = (float)font.height;
                float totalHeight = lineCount * lineHeight + (lineCount - 1) * LineSpacing + 2 * padding;

                Vector2 textRightPos = {
                    slider->bounds.x + slider->bounds.width + 10 + padding,
                    slider->bounds.y + (slider->bounds.height - totalHeight) / 2.0f + padding / 2.0f
                };

                Rectangle textRightBg = {
                    textRightPos.x - padding,
                    textRightPos.y - padding / 2.0f,
                    maxWidth + 2 * padding,
                    totalHeight
                };

                Color bgColor = Fade(slider->baseColor, 0.9f);
                Color borderColor = GetContrastingColor(bgColor);
                Color textColor = borderColor;

                DrawRectangleRec(textRightBg, bgColor);
                DrawRectangleLinesEx(textRightBg, 1, borderColor);

                for (int li = 0; li < lineCount; li++) {
                    DrawPSFText(font,
                                (int)(textRightBg.x + padding),
                                (int)(textRightBg.y + padding / 2 + li * (lineHeight + LineSpacing)),
                                lines[li], localSpacing, textColor);
                }
            }

            // --- Числове значення слайдера ---
            char valueText[16];
            if (slider->value >= slider->minValue && slider->value <= slider->maxValue) {
                snprintf(valueText, sizeof(valueText), "%.2f", slider->value);
            } else {
                snprintf(valueText, sizeof(valueText), "N/A");
            }

            int charCountVal = 0;
            const char *pv = valueText;
            while (*pv) {
                if ((*pv & 0xC0) != 0x80) charCountVal++;
                pv++;
            }

            float valTextWidth = charCountVal * (font.width + localSpacing) - localSpacing;
            float valBoxWidth = valTextWidth + 2 * padding;
            float valBoxHeight = font.height + 2 * padding;

            Rectangle valueRect = {
                slider->bounds.x + slider->bounds.width + 12,
                slider->bounds.y + slider->bounds.height / 2.0f + 20,
                valBoxWidth,
                valBoxHeight
            };

            Color bgColorVal = Fade(slider->baseColor, 0.9f);
            Color textColorVal = GetContrastingColor(bgColorVal);

            DrawRectangleRec(valueRect, bgColorVal);
            DrawRectangleLinesEx(valueRect, 1, textColorVal);
            DrawPSFText(font, valueRect.x + padding, valueRect.y + padding / 2, valueText, localSpacing, textColorVal);

            // При відображенні підказок зупиняємо і малюємо лише для одного слайдера
            break;
        }
    }
}

