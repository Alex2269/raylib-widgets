// slider_widget.c
// Віджет слайдерів з підтримкою множинних слайдерів,
// з активною ручкою, текстовими підказками та малюванням з контрастом.

#include "slider_widget.h"
#include <stdio.h>
#include <string.h>
#include <math.h>

// Максимальна кількість одночасно зареєстрованих слайдерів
#define MAX_SLIDERS 16

// Фізичний розмір ручки слайдера в пікселях
#define SLIDER_KNOB_SIZE 12

// Радіус "захоплення" мишею - розмір зони, де вважаємо, що миша "на ручці"
#define CAPTURE_RADIUS 10

// Структура, що описує один слайдер
typedef struct {
    Rectangle bounds;        // Область слайдера (позиція і розмір)
    float *value;            // Вказівник на значення слайдера (float)
    float minValue;          // Мінімум діапазону значень
    float maxValue;          // Максимум діапазону значень
    bool isVertical;         // Орієнтація: true - вертикальний, false - горизонтальний
    Color baseColor;         // Базовий колір слайдера (фон, ручка)
    bool isActive;           // Прапорець активності (чи захоплено мишею)
    bool used;               // Прапорець зайнятості даного слоту слайдерів
    const char* textTop;     // Підказка текстом над слайдером, може бути NULL
    const char* textRight;   // Підказка текстом праворуч, може бути NULL
} SliderEx;

// Масив зареєстрованих слайдерів
static SliderEx sliders[MAX_SLIDERS] = {0};

// Кількість слайдерів, які наразі зареєстровані
static int slidersCount = 0;

// Індекс активного слайдера (-1, якщо немає активного)
static int activeSliderIndex = -1;

// Зовнішня функція малювання тексту (PSF шрифт)
// Реалізована у зовнішньому модулі (не в цьому файлі)
extern void DrawPSFText(PSF_Font font, int x, int y, const char* text, int spacing, Color color);

// Глобальні змінні, що задають відступи для тексту
extern int LineSpacing;  // Відступ між рядками (пікселі)
extern int spacing;      // Відступ між символами (пікселі)

// Підрахунок кількості UTF-8 символів у рядку, а не байтів
/*
static int utf8_strlen(const char* s)
{
    int len = 0;
    while (*s) {
        // Байти, які починають символ UTF-8, мають старші біти не 10 (0x80-0xBF)
        if ((*s & 0xC0) != 0x80) len++;
        s++;
    }
    return len;
} */

// Зміна яскравості (насиченості) кольору: робить колір яскравішим або тьмянішим
static Color ChangeSaturation(Color c, float saturationScale) {
    float r = c.r / 255.0f;
    float g = c.g / 255.0f;
    float b = c.b / 255.0f;

    // Знаходимо максимум і мінімум компонент RGB для конвертації в HSV
    float cMax = fmaxf(r, fmaxf(g, b));
    float cMin = fminf(r, fminf(g, b));
    float delta = cMax - cMin;

    float h = 0.0f;  // відтінок
    if (delta > 0) {
        if (cMax == r) h = fmodf((g - b) / delta, 6.0f);
        else if (cMax == g) h = (b - r) / delta + 2.0f;
        else h = (r - g) / delta + 4.0f;
        h *= 60.0f;
        if (h < 0) h += 360.0f;
    }

    float s = (cMax == 0) ? 0 : delta / cMax; // насиченість
    float v = cMax;                           // яскравість

    s *= saturationScale;
    if (s > 1.0f) s = 1.0f;

    // Конвертація назад з HSV у RGB після зміни насиченості
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
    newColor.a = c.a; // зберігаємо прозорість
    return newColor;
}

// Перевіряє, чи миша знаходиться у «зоні захоплення» навколо ручки слайдера
static bool IsMouseNearKnob(Vector2 mousePos, SliderEx *slider) {
    // Якщо слайдер не використовується або вказівник на значення NULL – повертаємо false
    if (!slider->used || slider->value == NULL) return false;

    // Обчислюємо нормалізовану позицію ручки [0..1]
    float normValue = (*slider->value - slider->minValue) / (slider->maxValue - slider->minValue);
    float knobX, knobY;

    // Визначення координат ручки в залежності від орієнтації слайдера
    if (slider->isVertical) {
        knobX = slider->bounds.x + slider->bounds.width / 2.0f;
        knobY = slider->bounds.y + (1.0f - normValue) * slider->bounds.height;
    } else {
        knobX = slider->bounds.x + normValue * slider->bounds.width;
        knobY = slider->bounds.y + slider->bounds.height / 2.0f;
    }

    // Визначення чи миша у межах радіуса захоплення ( порівнюємо квадрат відстані)
    float dx = mousePos.x - knobX;
    float dy = mousePos.y - knobY;
    return (dx * dx + dy * dy) <= (CAPTURE_RADIUS * CAPTURE_RADIUS);
}

// Оновлення значення слайдера за позицією курсора миші
static void UpdateValueFromMouse(SliderEx *slider, Vector2 mousePos) {
    if (slider->value == NULL) return; // Захист від NULL

    float normValue;
    if (slider->isVertical)
        normValue = 1.0f - (mousePos.y - slider->bounds.y) / slider->bounds.height;
    else
        normValue = (mousePos.x - slider->bounds.x) / slider->bounds.width;

    // Кліпінг в межах [0..1]
    normValue = fmaxf(0.0f, fminf(1.0f, normValue));

    // Обчислення нового значення слайдера у реальному діапазоні
    *slider->value = slider->minValue + normValue * (slider->maxValue - slider->minValue);
}

// Повертає чорний або білий колір залежно від яскравості вхідного (для контрастності)
static Color GetContrastingColor(Color c) {
    float luminance = 0.2126f * c.r / 255.f + 0.7152f * c.g / 255.f + 0.0722f * c.b / 255.f;
    return (luminance > 0.5f) ? BLACK : WHITE;
}

// Функція реєстрації слайдера або оновлення параметрів існуючого
void RegisterSlider(int sliderIndex, Rectangle bounds, float *value, float minValue, float maxValue,
                    bool isVertical, Color baseColor, const char* textTop, const char* textRight) {
    if (sliderIndex < 0 || sliderIndex >= MAX_SLIDERS) return;

    // Якщо слот не зайнято, ініціалізуємо параметри слайдера
    if (!sliders[sliderIndex].used) {
        sliders[sliderIndex].bounds = bounds;
        sliders[sliderIndex].minValue = minValue;
        sliders[sliderIndex].maxValue = maxValue;
        sliders[sliderIndex].isVertical = isVertical;
        sliders[sliderIndex].baseColor = baseColor;
        sliders[sliderIndex].used = true;
        // Оновлюємо кількість зареєстрованих слайдерів, якщо індекс більший
        if (sliderIndex >= slidersCount) slidersCount = sliderIndex + 1;
    }

    // Оновлення поточного значення та текстових підказок
    sliders[sliderIndex].value = value;
    sliders[sliderIndex].textTop = textTop;
    sliders[sliderIndex].textRight = textRight;
    sliders[sliderIndex].isActive = false; // За замовчуванням слайдер не активний
}

// Головна функція: оновлення логіки і малювання всіх слайдерів
void UpdateSlidersAndDraw(PSF_Font font, int localSpacing) {
    Vector2 mousePos = GetMousePosition(); // Поточна позиція курсора

    // Якщо ліву кнопку миші натиснуто і немає активного слайдера, шукємо слайдер під мишкою
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && activeSliderIndex == -1) {
        for (int i = slidersCount - 1; i >= 0; i--) {
            if (sliders[i].used && IsMouseNearKnob(mousePos, &sliders[i])) {
                // Деактивуємо всі слайдери
                for (int k = 0; k < slidersCount; k++) sliders[k].isActive = false;

                // Активуємо слайдер під курсором
                activeSliderIndex = i;
                sliders[i].isActive = true;
                break;
            }
        }
    }
    // Якщо кнопку відпустили, скидаємо активність та індекс активного слайдера
    else if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
        for (int k = 0; k < slidersCount; k++) sliders[k].isActive = false;
        activeSliderIndex = -1;
    }

    // Якщо є активний слайдер і кнопку тримають, оновлюємо його значення
    if (activeSliderIndex != -1 && IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
        UpdateValueFromMouse(&sliders[activeSliderIndex], mousePos);
    }

    // Цикл малювання всіх слайдерів у порядку їхнього розташування
    for (int i = 0; i < slidersCount; i++) {
        SliderEx *slider = &sliders[i];
        if (!slider->used || slider->value == NULL) continue;

        // Обчислюємо нормалізоване значення в [0..1] для позиції ручки
        float normVal = (*slider->value - slider->minValue) / (slider->maxValue - slider->minValue);

        // Малюємо напівпрозорий фон слайдера
        DrawRectangleRec(slider->bounds, Fade(slider->baseColor, 0.20f));
        // Малюємо контрастну рамку слайдера
        DrawRectangleLinesEx(slider->bounds, 1, GetContrastingColor(slider->baseColor));

        // Якщо слайдер не активний, малюємо його ручку
        if (!slider->isActive) {
            Color knobColor = slider->baseColor;
            // Підсвічуємо ручку яскравіше, якщо миша наведена
            if (IsMouseNearKnob(mousePos, slider)) {
                knobColor = ChangeSaturation(slider->baseColor, 1.2f);
            }

            // Малюємо ручку: горизонтально або вертикально
            if (slider->isVertical) {
                float knobY = slider->bounds.y + (1.0f - normVal) * slider->bounds.height;
                DrawRectangle(slider->bounds.x, knobY - SLIDER_KNOB_SIZE / 2, slider->bounds.width, SLIDER_KNOB_SIZE, knobColor);
            } else {
                float knobX = slider->bounds.x + normVal * slider->bounds.width;
                DrawRectangle(knobX - SLIDER_KNOB_SIZE / 2, slider->bounds.y, SLIDER_KNOB_SIZE, slider->bounds.height, knobColor);
            }
        }
    }

    // Малюємо активний слайдер поверх інших з яскравішою ручкою
    if (activeSliderIndex != -1) {
        SliderEx *slider = &sliders[activeSliderIndex];
        float normVal = (*slider->value - slider->minValue) / (slider->maxValue - slider->minValue);

        Color knobColor = ChangeSaturation(slider->baseColor, 1.5f);
        if (slider->isVertical) {
            float knobY = slider->bounds.y + (1.0f - normVal) * slider->bounds.height;
            DrawRectangle(slider->bounds.x, knobY - SLIDER_KNOB_SIZE / 2, slider->bounds.width, SLIDER_KNOB_SIZE, knobColor);
        } else {
            float knobX = slider->bounds.x + normVal * slider->bounds.width;
            DrawRectangle(knobX - SLIDER_KNOB_SIZE / 2, slider->bounds.y, SLIDER_KNOB_SIZE, slider->bounds.height, knobColor);
        }
    }

    // Малюємо підказки і числові значення для першого активного або наведеного слайдера
    for (int i = slidersCount - 1; i >= 0; i--) {
        SliderEx* slider = &sliders[i];
        if (!slider->used) continue;

        // Перевіряємо, чи активний або наведений слайдер
        if (slider->isActive || IsMouseNearKnob(mousePos, slider)) {
            int padding = 6; // Внутрішній відступ текстових блоків

            // --- Підказка над слайдером ---
            if (slider->textTop && slider->textTop[0] != '\0') {
                const char* lines[10];
                int lineCount = 0;

                // Копіюємо текст у тимчасовий буфер для розбиття по рядках
                char tempText[256];
                strncpy(tempText, slider->textTop, sizeof(tempText) - 1);
                tempText[sizeof(tempText) - 1] = '\0';

                // Розбиваємо рядок тексту на підрядки по '\n'
                char* token = strtok(tempText, "\n");
                while (token != NULL && lineCount < 10) {
                    lines[lineCount++] = token;
                    token = strtok(NULL, "\n");
                }

                // Обчислюємо максимальну ширину серед рядків
                float maxWidth = 0;
                for (int li = 0; li < lineCount; li++) {
                    int charCount = utf8_strlen(lines[li]);
                    float lineWidth = charCount * (font.width + localSpacing) - localSpacing;
                    if (lineWidth > maxWidth) maxWidth = lineWidth;
                }

                // Визначаємо висоту одного рядка і загальну висоту блоку з текстом
                float lineHeight = (float)font.height;
                float totalHeight = lineCount * lineHeight + (lineCount - 1) * LineSpacing + 2 * padding;

                Rectangle tooltipRect = {
                    slider->bounds.x + slider->bounds.width / 2.0f - (maxWidth + 2 * padding) / 2.0f,
                    slider->bounds.y - totalHeight - 8,
                    maxWidth + 2 * padding,
                    totalHeight
                };

                // Малюємо фон та рамку
                DrawRectangleRec(tooltipRect, Fade(BLACK, 0.8f));
                DrawRectangleLinesEx(tooltipRect, 1, WHITE);

                // Малюємо кожен рядок тексту з відступами
                for (int li = 0; li < lineCount; li++) {
                    DrawPSFText(font,
                                (int)(tooltipRect.x + padding),
                                (int)(tooltipRect.y + padding / 2 + li * (lineHeight + LineSpacing)),
                                lines[li], localSpacing, WHITE);
                }
            }

            // --- Підказка праворуч від слайдера ---
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

                // Малюємо фон та рамку
                DrawRectangleRec(textRightBg, bgColor);
                DrawRectangleLinesEx(textRightBg, 1, borderColor);

                // Малюємо текст рядків
                for (int li = 0; li < lineCount; li++) {
                    DrawPSFText(font,
                                (int)(textRightBg.x + padding),
                                (int)(textRightBg.y + padding / 2 + li * (lineHeight + LineSpacing)),
                                lines[li], localSpacing, textColor);
                }
            }

            // --- Числове значення слайдера ---
            char valueText[16];
            if (slider->value != NULL) {
                snprintf(valueText, sizeof(valueText), "%.2f", *slider->value);
            } else {
                snprintf(valueText, sizeof(valueText), "N/A");
            }

            int charCountVal = 0;
            const char *pv = valueText;
            while (*pv) { if ((*pv & 0xC0) != 0x80) charCountVal++; pv++; }

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

            // Малюємо фон, рамку і текст числового значення слайдера
            DrawRectangleRec(valueRect, bgColorVal);
            DrawRectangleLinesEx(valueRect, 1, textColorVal);
            DrawPSFText(font, valueRect.x + padding, valueRect.y + padding / 2, valueText, localSpacing, textColorVal);

            // Малюємо підказки лише для одного слайдера
            break;
        }
    }
}

