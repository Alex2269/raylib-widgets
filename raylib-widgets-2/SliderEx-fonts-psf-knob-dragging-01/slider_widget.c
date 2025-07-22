// slider_widget.c

#include "slider_widget.h"
#include <stdio.h>
#include <string.h>
#include <math.h>

// Макроси та константи
#define MAX_SLIDERS 16       // Максимальна кількість слайдерів, що можуть бути зареєстровані
#define SLIDER_KNOB_SIZE 12  // Розмір ручки (курсора) слайдера в пікселях
#define CAPTURE_RADIUS 10    // Радіус захоплення миші навколо ручки

// Тип даних для зберігання стану одного слайдера
// Використовуємо структуру, щоб групувати всі параметри разом
typedef struct {
    Rectangle bounds;         // Прямокутна область слайдера
    float value;              // Поточне значення
    float minValue;           // Мінімальне значення
    float maxValue;           // Максимальне значення
    bool isVertical;          // Орієнтація: true для вертикального, false для горизонтального
    Color baseColor;          // Базовий колір
    bool isActive;            // Прапорець, що вказує, чи активний слайдер (захоплений мишею)
    bool used;                // Прапорець, що вказує, чи використовується слот в масиві
    const char* textTop;      // Текст над слайдером
    const char* textRight;    // Текст праворуч від слайдера
} SliderEx;

// Статичні глобальні змінні для зберігання стану всіх слайдерів.
// Вони доступні лише в межах цього файлу.
static SliderEx sliders[MAX_SLIDERS] = {0}; // Масив для зберігання всіх слайдерів
static int slidersCount = 0;                 // Лічильник зареєстрованих слайдерів
static int activeSliderIndex = -1;           // Індекс поточного активного слайдера (-1, якщо немає)

// --- Допоміжні функції ---

// Змінює насиченість кольору (на основі HSV-моделі).
// Дозволяє зробити колір яскравішим (при активації) або тьмянішим (при відпусканні).
static Color ChangeSaturation(Color c, float saturationScale) {
    float r = c.r / 255.0f;
    float g = c.g / 255.0f;
    float b = c.b / 255.0f;

    float cMax = fmaxf(r, fmaxf(g, b));
    float cMin = fminf(r, fminf(g, b));
    float delta = cMax - cMin;

    float h = 0.0f;
    if (delta > 0) {
        if (cMax == r) {
            h = fmodf((g - b) / delta, 6.0f);
        } else if (cMax == g) {
            h = (b - r) / delta + 2.0f;
        } else {
            h = (r - g) / delta + 4.0f;
        }
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
    newColor.a = c.a;
    return newColor;
}

// Перевіряє, чи знаходиться миша поблизу ручки слайдера (в межах CAPTURE_RADIUS).
static bool IsMouseNearKnob(Vector2 mousePos, SliderEx *slider) {
    if (!slider->used) return false;

    // Розраховуємо нормалізоване значення (від 0 до 1)
    float normValue = (slider->value - slider->minValue) / (slider->maxValue - slider->minValue);
    float knobX, knobY;

    // Визначаємо координати центру ручки залежно від орієнтації слайдера
    if (slider->isVertical) {
        knobX = slider->bounds.x + slider->bounds.width / 2.0f;
        knobY = slider->bounds.y + (1.0f - normValue) * slider->bounds.height;
    } else {
        knobX = slider->bounds.x + normValue * slider->bounds.width;
        knobY = slider->bounds.y + slider->bounds.height / 2.0f;
    }

    // Розраховуємо квадрат відстані між мишею та ручкою
    float dx = mousePos.x - knobX;
    float dy = mousePos.y - knobY;
    float distSq = dx * dx + dy * dy;

    // Повертаємо true, якщо відстань менша за радіус захоплення
    return (distSq <= CAPTURE_RADIUS * CAPTURE_RADIUS);
}

// Оновлює значення слайдера на основі поточної позиції миші.
static void UpdateValueFromMouse(SliderEx *slider, Vector2 mousePos) {
    float normValue;
    // Визначаємо нормалізоване значення залежно від орієнтації
    if (slider->isVertical) {
        normValue = 1.0f - (mousePos.y - slider->bounds.y) / slider->bounds.height;
    } else {
        normValue = (mousePos.x - slider->bounds.x) / slider->bounds.width;
    }
    // Обмежуємо значення в діапазоні [0, 1]
    normValue = fmaxf(0.0f, fminf(1.0f, normValue));
    // Перетворюємо нормалізоване значення у реальний діапазон
    slider->value = slider->minValue + normValue * (slider->maxValue - slider->minValue);
}

// Повертає колір, що контрастує з вхідним (чорний або білий).
static Color GetContrastingColor(Color c) {
    // Обчислюємо яскравість (luminance) кольору
    float luminance = 0.2126f * c.r / 255.0f + 0.7152f * c.g / 255.0f + 0.0722f * c.b / 255.0f;
    return (luminance > 0.5f) ? BLACK : WHITE;
}

// Зовнішня функція для малювання тексту.
// Ми припускаємо, що вона визначена в іншому місці (наприклад, psf_font.c).
extern void DrawPSFText(PSF_Font font, int x, int y, const char* text, int spacing, Color color);

// --- Основні функції віджетів ---

// Реєструє або ініціалізує слайдер у внутрішньому масиві.
// Ця функція викликається один раз за кадр для кожного слайдера.
void RegisterSlider(int sliderIndex, Rectangle bounds, float value, float minValue, float maxValue, bool isVertical, Color baseColor, const char* textTop, const char* textRight) {
    if (sliderIndex < 0 || sliderIndex >= MAX_SLIDERS) return;

    // Ініціалізуємо слайдер лише при першому виклику
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
        if (sliderIndex >= slidersCount) slidersCount = sliderIndex + 1;
    }
}

// Головна функція, що керує логікою та малюванням всіх слайдерів.
void UpdateSlidersAndDraw(PSF_Font font, int spacing) {
    Vector2 mousePos = GetMousePosition();

    // --- Обробка захоплення/звільнення миші ---
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        if (activeSliderIndex == -1) {
            // Перебираємо слайдери у зворотному порядку, щоб спочатку
            // обробляти ті, що знаходяться "зверху" (LIFO).
            for (int i = slidersCount - 1; i >= 0; i--) {
                if (IsMouseNearKnob(mousePos, &sliders[i])) {
                    activeSliderIndex = i;
                    sliders[i].isActive = true;
                    // Переміщуємо активний слайдер на останню позицію в масиві.
                    // Це гарантує, що він буде намальований останнім (поверх інших).
                    if (activeSliderIndex != slidersCount - 1) {
                        SliderEx temp = sliders[activeSliderIndex];
                        for (int j = activeSliderIndex; j < slidersCount - 1; j++) {
                            sliders[j] = sliders[j + 1];
                        }
                        sliders[slidersCount - 1] = temp;
                        activeSliderIndex = slidersCount - 1;
                    }
                    break; // Захоплюємо лише один слайдер
                }
            }
        }
    } else if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
        // Коли кнопку миші відпущено, деактивуємо поточний слайдер
        if (activeSliderIndex != -1) {
            sliders[activeSliderIndex].isActive = false;
            activeSliderIndex = -1;
        }
    }

    // Оновлюємо значення, якщо є активний слайдер
    if (activeSliderIndex != -1 && IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
        UpdateValueFromMouse(&sliders[activeSliderIndex], mousePos);
    }

    // --- Малювання всіх слайдерів ---
    // Малюємо їх у порядку, в якому вони знаходяться в масиві
    for (int i = 0; i < slidersCount; i++) {
        SliderEx *slider = &sliders[i];
        float normVal = (slider->value - slider->minValue) / (slider->maxValue - slider->minValue);

        // Малюємо основу слайдера
        DrawRectangleRec(slider->bounds, Fade(slider->baseColor, 0.20f));
        DrawRectangleLinesEx(slider->bounds, 1, GetContrastingColor(slider->baseColor));

        // Визначаємо колір ручки:
        // - Яскравіший, якщо активний.
        // - Трохи яскравіший, якщо миша наведена.
        // - Базовий, якщо нічого з переліченого.
        Color knobColor = slider->baseColor;
        if (slider->isActive) {
            knobColor = ChangeSaturation(slider->baseColor, 1.5f);
        } else if (IsMouseNearKnob(mousePos, slider)) {
            knobColor = ChangeSaturation(slider->baseColor, 1.2f);
        }

        // Малюємо ручку (курсор)
        if (slider->isVertical) {
            float knobY = slider->bounds.y + (1.0f - normVal) * slider->bounds.height;
            DrawRectangle(slider->bounds.x, knobY - SLIDER_KNOB_SIZE / 2, slider->bounds.width, SLIDER_KNOB_SIZE, knobColor);
        } else {
            float knobX = slider->bounds.x + normVal * slider->bounds.width;
            DrawRectangle(knobX - SLIDER_KNOB_SIZE / 2, slider->bounds.y, SLIDER_KNOB_SIZE, slider->bounds.height, knobColor);
        }
    }

    // --- Малювання підказок та тексту ---
    // Перебираємо слайдери у зворотному порядку, щоб малювати текст лише
    // для найвищого (активного або hovered) елемента.
    for (int i = slidersCount - 1; i >= 0; i--) {
        SliderEx* slider = &sliders[i];
        if (slider->isActive || IsMouseNearKnob(mousePos, slider)) {
            // Малювання тексту-підказки зверху
            if (slider->textTop && slider->textTop[0] != '\0') {
                int padding = 6;
                // Підрахунок символів UTF-8 для коректного вимірювання ширини тексту
                int charCount = 0;
                const char *p = slider->textTop;
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

                DrawRectangleRec(tooltipRect, Fade(GetContrastingColor(slider->baseColor), 0.9f));
                DrawRectangleLinesEx(tooltipRect, 1, GetContrastingColor(slider->baseColor));
                DrawPSFText(font, tooltipRect.x + padding, tooltipRect.y + padding / 2, slider->textTop, spacing, slider->baseColor);
            }

            // Малювання тексту-підказки праворуч
            if (slider->textRight && slider->textRight[0] != '\0') {
                int padding = 6;
                int charCount = 0;
                const char *p = slider->textRight;
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

                DrawRectangleRec(textRightRect, Fade(GetContrastingColor(slider->baseColor), 0.9f));
                DrawRectangleLinesEx(textRightRect, 1, GetContrastingColor(slider->baseColor));
                DrawPSFText(font, textRightRect.x + padding, textRightRect.y + padding / 2, slider->textRight, spacing, GetContrastingColor(slider->baseColor));
            }

            // Малювання числового значення слайдера
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

            DrawRectangleRec(valueRect, Fade(slider->baseColor, 0.9f));
            DrawRectangleLinesEx(valueRect, 1, GetContrastingColor(slider->baseColor));
            DrawPSFText(font, valueRect.x + paddingVal, valueRect.y + paddingVal / 2, valueText, spacing, GetContrastingColor(slider->baseColor));

            break; // Малюємо підказки тільки для найвищого елементу
        }
    }
}

// Повертає поточне значення слайдера за його індексом.
float GetSliderValue(int sliderIndex) {
    // Перевіряємо, чи індекс є коректним і чи використовується слот.
    if (sliderIndex >= 0 && sliderIndex < slidersCount && sliders[sliderIndex].used) {
        return sliders[sliderIndex].value;
    }
    return 0.0f; // Повертаємо 0.0f у разі помилки
}
