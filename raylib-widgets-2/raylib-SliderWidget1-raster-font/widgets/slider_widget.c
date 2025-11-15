// slider_widget.c
// Віджет слайдерів з підтримкою множинних слайдерів,
// активною ручкою, підказками та контрастним малюванням.

#include "slider_widget.h"
#include <stdio.h>
#include <string.h>
#include <math.h>

// Максимальна кількість слайдерів, які можна зареєструвати
#define MAX_SLIDERS 16

// Розмір ручки слайдера (в пікселях)
#define SLIDER_KNOB_SIZE 10

// Радіус захоплення мишею ручки слайдера (в пікселях)
#define CAPTURE_RADIUS 15

// Глобальні змінні-налаштування для відступів (як і в guicheckbox.c)
extern int LineSpacing;  // Відступ між рядками (пікселі)
extern int spacing;      // Відступ між символами (пікселі)

// Структура, що зберігає параметри одного слайдера
typedef struct {
    Rectangle bounds;        // Область слайдера на екрані
    float *value;            // Вказівник на значення слайдера
    float minValue;          // Мінімальне значення
    float maxValue;          // Максимальне значення
    bool isVertical;         // Орієнтація: true - вертикальний, false - горизонтальний
    Color baseColor;         // Базовий колір слайдера
    bool isActive;           // Чи активний слайдер (захоплений мишею)
    bool used;               // Чи використовується слот у масиві
    const char* textTop;     // Текст над слайдером
    const char* textRight;   // Текст праворуч від слайдера
} SliderEx;

// Масив для зберігання всіх слайдерів
static SliderEx sliders[MAX_SLIDERS] = {0};

// Кількість зареєстрованих слайдерів
static int slidersCount = 0;

// Індекс активного слайдера (-1, якщо немає)
static int activeSliderIndex = -1;

// Перевіряє, чи миша знаходиться поблизу ручки слайдера (в межах CAPTURE_RADIUS)
static bool IsMouseNearKnob(Vector2 mousePos, SliderEx *slider) {
    if (!slider->used || slider->value == NULL) return false;

    // Нормалізоване значення (0..1)
    float normValue = (*slider->value - slider->minValue) / (slider->maxValue - slider->minValue);
    float knobX, knobY;

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

    return (distSq <= CAPTURE_RADIUS * CAPTURE_RADIUS);
}

// Оновлює значення слайдера на основі позиції миші
static void UpdateValueFromMouse(SliderEx *slider, Vector2 mousePos) {
    if (slider->value == NULL) return;

    float normValue;
    if (slider->isVertical) {
        normValue = 1.0f - (mousePos.y - slider->bounds.y) / slider->bounds.height;
    } else {
        normValue = (mousePos.x - slider->bounds.x) / slider->bounds.width;
    }
    normValue = fmaxf(0.0f, fminf(1.0f, normValue));
    *slider->value = slider->minValue + normValue * (slider->maxValue - slider->minValue);
}

// Реєструє слайдер або оновлює його параметри
void RegisterSlider(int sliderIndex, Rectangle bounds, RasterFont font, int spacing, const char* textTop, const char* textRight, float *value, float minValue, float maxValue, bool isVertical, Color baseColor) {
    if (sliderIndex < 0 || sliderIndex >= MAX_SLIDERS) return;

    if (!sliders[sliderIndex].used) {
        sliders[sliderIndex].bounds = bounds;
        sliders[sliderIndex].minValue = minValue;
        sliders[sliderIndex].maxValue = maxValue;
        sliders[sliderIndex].isVertical = isVertical;
        sliders[sliderIndex].baseColor = baseColor;
        sliders[sliderIndex].used = true;
        if (sliderIndex >= slidersCount) slidersCount = sliderIndex + 1;
    }
    sliders[sliderIndex].value = value;
    sliders[sliderIndex].textTop = textTop;
    sliders[sliderIndex].textRight = textRight;
    UpdateSlidersAndDraw(font, spacing);
}

// Основна функція оновлення та малювання слайдерів
void UpdateSlidersAndDraw(RasterFont font, int spacing) {
    Vector2 mousePos = GetMousePosition();

    // Обробка натискання миші: визначаємо активний слайдер
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && activeSliderIndex == -1) {
        for (int i = slidersCount - 1; i >= 0; i--) {
            if (sliders[i].used && IsMouseNearKnob(mousePos, &sliders[i])) {
                // Скидаємо активність у всіх слайдерів
                for (int k = 0; k < slidersCount; k++) sliders[k].isActive = false;
                activeSliderIndex = i;
                sliders[i].isActive = true;
                break;
            }
        }
    }
    // Обробка відпускання кнопки миші: скидаємо активність
    else if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
        for (int k = 0; k < slidersCount; k++) sliders[k].isActive = false;
        activeSliderIndex = -1;
    }

    // Оновлення значення активного слайдера під час руху миші
    if (activeSliderIndex != -1 && IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
        UpdateValueFromMouse(&sliders[activeSliderIndex], mousePos);
    }

    // Локальні відступи для тексту
    int localSpacing = spacing;
    float lineSpacing = (float)LineSpacing;

    // Малювання слайдерів (фони, рамки, неактивні ручки)
    for (int i = 0; i < slidersCount; i++) {
        SliderEx *slider = &sliders[i];
        if (!slider->used || slider->value == NULL) continue;

        float normVal = (*slider->value - slider->minValue) / (slider->maxValue - slider->minValue);

        // Малюємо фон та рамку слайдера
        DrawRectangleRec(slider->bounds, Fade(slider->baseColor, 0.20f));
        DrawRectangleLinesEx(slider->bounds, 1, GetContrastColor(slider->baseColor));

        // Малюємо ручку, якщо слайдер не активний
        if (!slider->isActive) {
            Color knobColor = slider->baseColor;
            if (IsMouseNearKnob(mousePos, slider)) {
                knobColor = ChangeSaturation(slider->baseColor, 1.2f);
            }
            if (slider->isVertical) {
                float knobY = slider->bounds.y + (1.0f - normVal) * slider->bounds.height;
                DrawRectangle(slider->bounds.x, knobY - SLIDER_KNOB_SIZE / 2, slider->bounds.width, SLIDER_KNOB_SIZE, knobColor);
            } else {
                float knobX = slider->bounds.x + normVal * slider->bounds.width;
                DrawRectangle(knobX - SLIDER_KNOB_SIZE / 2, slider->bounds.y, SLIDER_KNOB_SIZE, slider->bounds.height, knobColor);
            }
        }
    }

    // Малювання активної ручки зверху (щоб не перекривалася іншими)
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

    // Малювання текстів і значень для активного або наведенного слайдера
    for (int i = slidersCount - 1; i >= 0; i--) {
        SliderEx* slider = &sliders[i];
        if (!slider->used) continue;

        if (slider->isActive || IsMouseNearKnob(mousePos, slider)) {
            int padding = 6; // Відступи всередині блоків тексту

            // --- Малюємо текст над слайдером (textTop) ---
            if (slider->textTop && slider->textTop[0] != '\0')
            {
                // Обчислюємо довжину тексту в символах (utf8_strlen коректно рахує багатобайтові символи)
                int textLen = utf8_strlen(slider->textTop);

                // Ширина тексту з урахуванням ширини символу і проміжків між ними
                int textWidth = textLen * (font.glyph_width + localSpacing) - localSpacing;

                // Позиція по X: центруємо текст по горизонталі над слайдером
                int textX = slider->bounds.x + (slider->bounds.width / 2) - (textWidth / 2);
                // Позиція по Y: розміщуємо текст над слайдером, враховуючи висоту рядка і паддінг
                int textY = slider->bounds.y - font.glyph_height - padding * 2;

                // Малюємо текст з автоматично підібраним контрастним фоном і рамкою
                DrawTextWithAutoInvertedBackground(font, textX, textY,
                                                   slider->textTop,
                                                   localSpacing, 1,
                                                   slider->baseColor,
                                                   padding, 1);
            }

            // --- Малюємо текст праворуч від слайдера (textRight) ---
            if (slider->textRight && slider->textRight[0] != '\0')
            {
                int textLen = utf8_strlen(slider->textRight);
                int textWidth = textLen * (font.glyph_width + localSpacing) - localSpacing;

                // Позиція X: праворуч від слайдера з поправкою на паддінг і відступ
                int textRightX = slider->bounds.x + slider->bounds.width + 10 + padding;
                // Позиція Y: вертикально вирівнюємо текст по центру висоти слайдера
                int textRightY = slider->bounds.y + (slider->bounds.height / 2) - font.glyph_height;

                // Малюємо текст із фоном і рамкою, які контрастують з базовим кольором слайдера
                DrawTextWithAutoInvertedBackground(font, textRightX, textRightY,
                                                   slider->textRight,
                                                   localSpacing, 1,
                                                   slider->baseColor,
                                                   padding, 1);
            }

            // --- Виводимо числове значення слайдера ---
            {
                // Форматуємо числове значення з двома знаками після коми
                char valueText[16];
                snprintf(valueText, sizeof(valueText), "%.2f", *slider->value);

                // Обчислюємо довжину і ширину тексту для точного позиціонування
                int textLen = utf8_strlen(valueText);
                int textWidth = textLen * (font.glyph_width + localSpacing) - localSpacing;

                // Позиція X: праворуч від слайдера з невеликим відступом
                int textX = slider->bounds.x + slider->bounds.width + 12;
                // Позиція Y: трохи нижче центру слайдера, щоб не накладатися з textRight
                int textY = slider->bounds.y + (slider->bounds.height / 2.0f) + 20;

                // Малюємо числове значення з автоматично підібраним контрастним фоном і рамкою
                DrawTextWithAutoInvertedBackground(font, textX, textY,
                                                   valueText,
                                                   localSpacing, 1,
                                                   slider->baseColor,
                                                   padding, 1);
            }

            break;
        }
    }
}

