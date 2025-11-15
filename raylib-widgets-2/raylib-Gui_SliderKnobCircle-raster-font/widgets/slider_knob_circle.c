// slider_knob_circle.c

#include "slider_knob_circle.h"
#include <stdio.h>
#include <string.h>
#include <math.h>

// Максимальна кількість слайдерів, які підтримуємо
#define MAX_SLIDERS 16

// Радіус круглої ручки в пікселях
#define SLIDER_KNOB_RADIUS 12

// Зона в пікселях навколо ручки, в якій вважаємо мишу "біля ручки"
#define CAPTURE_RADIUS 12

// Глобальні змінні-налаштування для відступів (як і в guicheckbox.c)
extern int LineSpacing;  // Відступ між рядками (пікселі)
extern int spacing;      // Відступ між символами (пікселі)

// Опис структури слайдера з круглою ручкою
typedef struct {
    Rectangle bounds;        // Позиція і розмір слайдера у вікні (в пікселях)
    float *value;            // Вказівник на значення (float), яке редагує слайдер
    float minValue;          // Мінімальне значення слайдера
    float maxValue;          // Максимальне значення слайдера
    bool isVertical;         // Орієнтація: true – вертикальний, false – горизонтальний
    Color baseColor;         // Основний колір елемента слайдера
    bool isActive;           // Чи захоплена ручка мишею (активний)
    bool used;               // Чи використовується слот слайдера
    const char* textTop;     // Текст над слайдером (підказка), може бути NULL
    const char* textRight;   // Текст праворуч від слайдера, може бути NULL
} CircleSlider;

// Масив для зберігання всіх слайдерів
static CircleSlider sliders[MAX_SLIDERS] = {0};
static int slidersCount = 0;         // Скільки слайдерів зареєстровано
static int activeSliderIndex = -1;   // Індекс слайдера, який зараз перетягують (-1 якщо нема)

// Перевірка, чи миша перебуває в зоні захоплення ручки слайдера
static bool IsMouseNearCircleKnob(Vector2 mousePos, CircleSlider *slider)
{
    if (!slider->used || slider->value == NULL) return false;

    // Обчислюємо нормалізоване значення слайдера в діапазоні [0..1]
    float normValue = (*slider->value - slider->minValue) / (slider->maxValue - slider->minValue);

    float knobX, knobY;
    if (slider->isVertical) {
        knobX = slider->bounds.x + slider->bounds.width / 2.0f;         // по центру ширини слайдера
        knobY = slider->bounds.y + (1.0f - normValue) * slider->bounds.height; // по висоті від низу до верху
    } else {
        knobX = slider->bounds.x + normValue * slider->bounds.width;    // по ширині від лівого краю
        knobY = slider->bounds.y + slider->bounds.height / 2.0f;        // по центру висоти
    }

    // Відстань від позиції миші до центру ручки
    float dx = mousePos.x - knobX;
    float dy = mousePos.y - knobY;

    // Порівнюємо квадрат відстані з квадратом радіуса захоплення
    return (dx*dx + dy*dy) <= (CAPTURE_RADIUS * CAPTURE_RADIUS);
}

// Оновлення значення слайдера згідно позиції миші
static void UpdateValueFromMouseCircle(CircleSlider *slider, Vector2 mousePos)
{
    if (slider->value == NULL) return;

    // Обчислення нормалізованої позиції [0..1] залежно від орієнтації
    float normValue;

    if (slider->isVertical) {
        normValue = 1.0f - (mousePos.y - slider->bounds.y) / slider->bounds.height;
    } else {
        normValue = (mousePos.x - slider->bounds.x) / slider->bounds.width;
    }

    // Обрізаємо, щоб не вийшло поза межі
    if (normValue < 0.0f) normValue = 0.0f;
    if (normValue > 1.0f) normValue = 1.0f;

    // Обчислюємо реальне значення слайдера відповідно до діапазону
    *slider->value = slider->minValue + normValue * (slider->maxValue - slider->minValue);
}

// Функція реєстрації або оновлення слайдера в масиві
void Gui_SliderKnobCircle(int sliderIndex, Rectangle bounds,
                              RasterFont font, int spacing,
                              const char* textTop, const char* textRight,
                              float *value, float minValue, float maxValue,
                              bool isVertical, Color baseColor)
{
    // Перевірка на вихід за межі масиву
    if (sliderIndex < 0 || sliderIndex >= MAX_SLIDERS) return;

    if (!sliders[sliderIndex].used) {
        // Заповнюємо основні параметри слайдера
        sliders[sliderIndex].bounds = bounds;
        sliders[sliderIndex].minValue = minValue;
        sliders[sliderIndex].maxValue = maxValue;
        sliders[sliderIndex].isVertical = isVertical;
        sliders[sliderIndex].baseColor = baseColor;
        sliders[sliderIndex].used = true;

        // Оновлюємо загальну кількість слайдерів, якщо індекс більший за поточний максимум
        if (sliderIndex >= slidersCount)
            slidersCount = sliderIndex + 1;
    }

    // Оновлюємо значення, тексти та стан активності
    sliders[sliderIndex].value = value;
    sliders[sliderIndex].textTop = textTop;
    sliders[sliderIndex].textRight = textRight;
    sliders[sliderIndex].isActive = false;
    UpdateCircleKnobSlidersAndDraw(font, spacing);
}

// Функція малювання круглої ручки слайдера
static void DrawCircleKnob(float centerX, float centerY, float radius, Color color)
{
    DrawCircle((int)centerX, (int)centerY, radius, color);
}
// Глобальна змінна - останній активний слайдер (для малювання зверху)
static CircleSlider* lastActiveSliderPtr = NULL;

// Глобальна змінна - актуальний слайдер, який зараз перетягують
static CircleSlider* activeSliderPtr = NULL;

static void TryActivateSlider(Vector2 mousePos)
{
    if (activeSliderPtr != NULL) return;

    for (int i = slidersCount - 1; i >= 0; i--)
    {
        if (sliders[i].used && IsMouseNearCircleKnob(mousePos, &sliders[i]))
        {
            activeSliderPtr = &sliders[i];
            lastActiveSliderPtr = activeSliderPtr; // Запам'ятовуємо останній активний

            for (int k = 0; k < slidersCount; k++) sliders[k].isActive = false;
            activeSliderPtr->isActive = true;
            break;
        }
    }
}

void UpdateCircleKnobSlidersAndDraw(RasterFont font, int spacing)
{
    Vector2 mousePos = GetMousePosition();

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        TryActivateSlider(mousePos);
    }

    if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
        if (activeSliderPtr) activeSliderPtr->isActive = false;
        activeSliderPtr = NULL; // Зупиняємо перетягування, але зберігаємо lastActiveSliderPtr
    }

    if (activeSliderPtr != NULL && IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
        UpdateValueFromMouseCircle(activeSliderPtr, mousePos);
    }

    // Малюємо всі слайдери, крім останнього активного (lastActiveSliderPtr)
    for (int i = 0; i < slidersCount; i++) {
        CircleSlider* slider = &sliders[i];
        if (!slider->used || slider->value == NULL) continue;
        if (slider == lastActiveSliderPtr) continue; // Пропускаємо останній активний

        DrawRectangleRec(slider->bounds, Fade(slider->baseColor, 0.20f));
        DrawRectangleLinesEx(slider->bounds, 1, GetContrastColor(slider->baseColor));

        Color knobColor = slider->baseColor;
        if (!slider->isActive && IsMouseNearCircleKnob(mousePos, slider)) {
            knobColor = ChangeSaturation(slider->baseColor, 1.2f);
        }

        float normVal = (*slider->value - slider->minValue) / (slider->maxValue - slider->minValue);
        float knobCenterX, knobCenterY;
        if (slider->isVertical) {
            knobCenterX = slider->bounds.x + slider->bounds.width / 2.0f;
            knobCenterY = slider->bounds.y + (1.0f - normVal) * slider->bounds.height;
        } else {
            knobCenterX = slider->bounds.x + normVal * slider->bounds.width;
            knobCenterY = slider->bounds.y + slider->bounds.height / 2.0f;
        }
        DrawCircleKnob(knobCenterX, knobCenterY, SLIDER_KNOB_RADIUS, knobColor);
    }

    // Малюємо останній активний слайдер зверху, якщо він є
    if (lastActiveSliderPtr != NULL) {
        DrawRectangleRec(lastActiveSliderPtr->bounds, Fade(lastActiveSliderPtr->baseColor, 0.20f));
        DrawRectangleLinesEx(lastActiveSliderPtr->bounds, 1, GetContrastColor(lastActiveSliderPtr->baseColor));

        float normVal = (*lastActiveSliderPtr->value - lastActiveSliderPtr->minValue) / (lastActiveSliderPtr->maxValue - lastActiveSliderPtr->minValue);
        Color knobColor = ChangeSaturation(lastActiveSliderPtr->baseColor, 1.5f);

        float knobCenterX, knobCenterY;
        if (lastActiveSliderPtr->isVertical) {
            knobCenterX = lastActiveSliderPtr->bounds.x + lastActiveSliderPtr->bounds.width / 2.0f;
            knobCenterY = lastActiveSliderPtr->bounds.y + (1.0f - normVal) * lastActiveSliderPtr->bounds.height;
        } else {
            knobCenterX = lastActiveSliderPtr->bounds.x + normVal * lastActiveSliderPtr->bounds.width;
            knobCenterY = lastActiveSliderPtr->bounds.y + lastActiveSliderPtr->bounds.height / 2.0f;
        }
        DrawCircleKnob(knobCenterX, knobCenterY, SLIDER_KNOB_RADIUS, knobColor);

        // Тут можна домалювати тултіпи, текст або значення слайдера
        Vector2 center = { (float)knobCenterX, (float)knobCenterY };     // Центр rotary cam switch
        Vector2 mousePos = GetMousePosition();               // Поточна позиція курсора миші

        // Перевіряємо чи курсор знаходиться в межах радиуса rotary cam switch
        bool mouseOver = CheckCollisionPointCircle(mousePos, center, SLIDER_KNOB_RADIUS);

        if (mouseOver) {
            if (lastActiveSliderPtr != NULL)
            {
                // Малюємо фон і рамку самого слайдера (як раніше)
                DrawRectangleRec(lastActiveSliderPtr->bounds, Fade(lastActiveSliderPtr->baseColor, 0.20f));
                DrawRectangleLinesEx(lastActiveSliderPtr->bounds, 1, GetContrastColor(lastActiveSliderPtr->baseColor));

                float normVal = (*lastActiveSliderPtr->value - lastActiveSliderPtr->minValue) / (lastActiveSliderPtr->maxValue - lastActiveSliderPtr->minValue);
                Color knobColor = ChangeSaturation(lastActiveSliderPtr->baseColor, 1.5f);

                float knobCenterX, knobCenterY;
                if (lastActiveSliderPtr->isVertical) {
                    knobCenterX = lastActiveSliderPtr->bounds.x + lastActiveSliderPtr->bounds.width / 2.0f;
                    knobCenterY = lastActiveSliderPtr->bounds.y + (1.0f - normVal) * lastActiveSliderPtr->bounds.height;
                } else {
                    knobCenterX = lastActiveSliderPtr->bounds.x + normVal * lastActiveSliderPtr->bounds.width;
                    knobCenterY = lastActiveSliderPtr->bounds.y + lastActiveSliderPtr->bounds.height / 2.0f;
                }
                DrawCircleKnob(knobCenterX, knobCenterY, SLIDER_KNOB_RADIUS, knobColor);

                // --- Малюємо текст ---
                int localSpacing = spacing;               // Відступ між символами тексту
                float lineSpacing = (float)LineSpacing;  // Відступ між рядками тексту

                // Відступи для фону та рамки текстових блоків
                const int padding = 6;        // Відступ між текстом і краєм прямокутника
                const int borderThickness = 1; // Товщина рамки текстового прямокутника

                // --- Малюємо текст над слайдером (textTop) ---
                if (lastActiveSliderPtr->textTop && lastActiveSliderPtr->textTop[0] != '\0')
                {
                    // Обчислюємо довжину тексту в символах (utf8_strlen коректно рахує багатобайтові символи)
                    int textLen = utf8_strlen(lastActiveSliderPtr->textTop);

                    // Ширина тексту з урахуванням ширини символу і проміжків між ними
                    int textWidth = textLen * (font.glyph_width + localSpacing) - localSpacing;

                    // Позиція по X: центруємо текст по горизонталі над слайдером
                    int textX = lastActiveSliderPtr->bounds.x + (lastActiveSliderPtr->bounds.width / 2) - (textWidth / 2);
                    // Позиція по Y: розміщуємо текст над слайдером, враховуючи висоту рядка і паддінг
                    int textY = lastActiveSliderPtr->bounds.y - font.glyph_height - padding * 2;

                    // Малюємо текст з автоматично підібраним контрастним фоном і рамкою
                    DrawTextWithAutoInvertedBackground(font, textX, textY,
                                                       lastActiveSliderPtr->textTop,
                                                       localSpacing, 1,
                                                       lastActiveSliderPtr->baseColor,
                                                       padding, borderThickness);
                }

                // --- Малюємо текст праворуч від слайдера (textRight) ---
                if (lastActiveSliderPtr->textRight && lastActiveSliderPtr->textRight[0] != '\0')
                {
                    int textLen = utf8_strlen(lastActiveSliderPtr->textRight);
                    int textWidth = textLen * (font.glyph_width + localSpacing) - localSpacing;

                    // Позиція X: праворуч від слайдера з поправкою на паддінг і відступ
                    int textRightX = lastActiveSliderPtr->bounds.x + lastActiveSliderPtr->bounds.width + 10 + padding;
                    // Позиція Y: вертикально вирівнюємо текст по центру висоти слайдера
                    int textRightY = lastActiveSliderPtr->bounds.y + (lastActiveSliderPtr->bounds.height / 2) - font.glyph_height;

                    // Малюємо текст із фоном і рамкою, які контрастують з базовим кольором слайдера
                    DrawTextWithAutoInvertedBackground(font, textRightX, textRightY,
                                                       lastActiveSliderPtr->textRight,
                                                       localSpacing, 1,
                                                       lastActiveSliderPtr->baseColor,
                                                       padding, borderThickness);
                }

                // --- Виводимо числове значення слайдера ---
                {
                    // Форматуємо числове значення з двома знаками після коми
                    char valueText[16];
                    snprintf(valueText, sizeof(valueText), "%.2f", *lastActiveSliderPtr->value);

                    // Обчислюємо довжину і ширину тексту для точного позиціонування
                    int textLen = utf8_strlen(valueText);
                    int textWidth = textLen * (font.glyph_width + localSpacing) - localSpacing;

                    // Позиція X: праворуч від слайдера з невеликим відступом
                    int textX = lastActiveSliderPtr->bounds.x + lastActiveSliderPtr->bounds.width + 12;
                    // Позиція Y: трохи нижче центру слайдера, щоб не накладатися з textRight
                    int textY = lastActiveSliderPtr->bounds.y + (lastActiveSliderPtr->bounds.height / 2.0f) + 20;

                    // Малюємо числове значення з автоматично підібраним контрастним фоном і рамкою
                    DrawTextWithAutoInvertedBackground(font, textX, textY,
                                                       valueText,
                                                       localSpacing, 1,
                                                       lastActiveSliderPtr->baseColor,
                                                       padding, borderThickness);
                }
            }
        }
    }
}
