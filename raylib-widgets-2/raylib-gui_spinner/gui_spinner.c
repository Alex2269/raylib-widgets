// gui_spinner.c
#include "gui_spinner.h"
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <math.h> // для roundf()

// Використання roundf() для коректного округлення float-значень до кратності кроку у функції UpdateSliderValue.
// Алтернатива roundf(), наразі не використовується.
static float roundf_alt(float x) {
    return (float)( (x > 0) ? (int)(x + 0.5f) : (int)(x - 0.5f) );
}

// Структура для збереження стану утримання кнопки (автоповтор при тривалому натисканні)
typedef struct {
    bool isHeld;            // Чи наразі кнопка утримується
    double holdStartTime;   // Час початку утримання в секундах
    double lastUpdateTime;  // Час останнього оновлення (секунди)
    double accumulatedTime; // Накопичений час для повторення кроку (секунди)
} HoldState;

// Внутрішній ліміт на кількість спінерів
#define MAX_SPINNERS 16

// Внутрішні масиви для станів утримання лівих і правих кнопок спінерів
static HoldState holdLeftStates[MAX_SPINNERS] = {{0}};
static HoldState holdRightStates[MAX_SPINNERS] = {{0}};

// Отримати системний час з високою точністю (монотоне джерело часу)
static double GetSystemTime() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (double)ts.tv_sec + ts.tv_nsec / 1e9;
}

// Повернути контрастний колір (чорний або білий) залежно від яскравості кольору
static Color GetContrastingColor(Color c) {
    float luminance = 0.2126f * c.r / 255 + 0.7152f * c.g / 255 + 0.0722f * c.b / 255;
    return (luminance > 0.5f) ? BLACK : WHITE;
}

// Напрямок стрілки кнопок
typedef enum {
    ARROW_LEFT,
    ARROW_RIGHT
} ArrowDirection;

// Малювання трикутної стрілки у межах кнопки
static void DrawArrow(Rectangle bounds, ArrowDirection dir, Color color) {
    Vector2 center = { bounds.x + bounds.width/2.0f, bounds.y + bounds.height/2.0f };
    float size = bounds.width * 0.45f;
    Vector2 points[3];

    switch (dir) {
        case ARROW_LEFT:
            points[0] = (Vector2){center.x - size, center.y};
            points[1] = (Vector2){center.x + size, center.y + size};
            points[2] = (Vector2){center.x + size, center.y - size};
            break;
        case ARROW_RIGHT:
            points[0] = (Vector2){center.x + size, center.y};
            points[1] = (Vector2){center.x - size, center.y - size};
            points[2] = (Vector2){center.x - size, center.y + size};
            break;
    }
    DrawTriangle(points[0], points[1], points[2], color);
}

// Кнопка зі стрілкою з логікою натискання та утримання
// Оновлює *value залежно від натискання та типу значення
static bool ArrowButton(Rectangle bounds, ArrowDirection dir,
                        void* value, void* minVal, void* maxVal,
                        float step, GuiSpinnerValueType valueType,
                        HoldState* holdState, Color baseColor)
{
    Vector2 mousePos = GetMousePosition();
    bool mouseOver = CheckCollisionPointRec(mousePos, bounds);
    bool changed = false;

    // Визначаємо колір кнопки в залежності від стану миші
    Color btnColor = baseColor;
    if (mouseOver) btnColor = Fade(baseColor, 0.8f);
    if (mouseOver && IsMouseButtonDown(MOUSE_LEFT_BUTTON)) btnColor = Fade(baseColor, 0.6f);

    Color borderColor = GetContrastingColor(btnColor);
    const int borderThickness = 2;

    // Малюємо кнопку
    DrawRectangleLinesEx((Rectangle){bounds.x - borderThickness, bounds.y - borderThickness,
                                     bounds.width + 2 * borderThickness, bounds.height + 2 * borderThickness},
                                     borderThickness, borderColor);
    DrawRectangleRec(bounds, btnColor);
    DrawArrow(bounds, dir, GetContrastingColor(btnColor));

    double now = GetSystemTime();

    // Якщо кнопка не утримується, ініціалізуємо часові змінні
    if (!holdState->isHeld) {
        holdState->lastUpdateTime = now;
        holdState->accumulatedTime = 0.0;
    }

    double deltaTime = now - holdState->lastUpdateTime;
    holdState->lastUpdateTime = now;

    bool mousePressed = mouseOver && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
    bool mouseDown = mouseOver && IsMouseButtonDown(MOUSE_LEFT_BUTTON);

    // Параметри для затримки і прискорення автоповтору при утриманні кнопки
    const double delayBeforeAccel = 0.175;
    const double baseInterval = 0.25;
    const double minInterval = 0.005;
    const double accelRate = 0.075;

    // Організація зміни значення через допоміжні макроси
#define INCREMENT_VAL()                                           \
    do {                                                        \
        if (valueType == GUI_SPINNER_FLOAT) {                   \
            float *v = (float*)value;                           \
            float maxV = *(float*)maxVal;                       \
            if (*v + step <= maxV) {                            \
                *v += step;                                     \
                changed = true;                                 \
            }                                                  \
        } else {                                               \
            int *v = (int*)value;                              \
            int maxV = *(int*)maxVal;                          \
            int stepInt = (int)(step + 0.5f); /* округлення */ \
            if (*v + stepInt <= maxV) {                        \
                *v += stepInt;                                 \
                changed = true;                                 \
            }                                                  \
        }                                                      \
    } while (0)

#define DECREMENT_VAL()                                           \
    do {                                                        \
        if (valueType == GUI_SPINNER_FLOAT) {                   \
            float *v = (float*)value;                           \
            float minV = *(float*)minVal;                       \
            if (*v - step >= minV) {                            \
                *v -= step;                                     \
                changed = true;                                 \
            }                                                  \
        } else {                                               \
            int *v = (int*)value;                              \
            int minV = *(int*)minVal;                          \
            int stepInt = (int)(step + 0.5f); /* округлення */ \
            if (*v - stepInt >= minV) {                        \
                *v -= stepInt;                                 \
                changed = true;                                 \
            }                                                  \
        }                                                      \
    } while (0)

    if (mousePressed) {
        holdState->isHeld = true;
        holdState->holdStartTime = now;
        holdState->accumulatedTime = 0.0;

        // Виконуємо перший крок зміни відповідно до напряму стрілки
        if (dir == ARROW_RIGHT)
            INCREMENT_VAL();
        else
            DECREMENT_VAL();
    }
    else if (mouseDown && holdState->isHeld) {
        // Логіка автоповтору при утриманні кнопки із прискоренням
        double holdDuration = now - holdState->holdStartTime;
        double interval = baseInterval;

        if (holdDuration > delayBeforeAccel) {
            double accelTime = holdDuration - delayBeforeAccel;
            interval = baseInterval - accelTime * accelRate;
            if (interval < minInterval) interval = minInterval;
        }

        holdState->accumulatedTime += deltaTime;

        while (holdState->accumulatedTime >= interval) {
            holdState->accumulatedTime -= interval;
            if (dir == ARROW_RIGHT)
                INCREMENT_VAL();
            else
                DECREMENT_VAL();
        }
    } else {
        // Кнопка відпущена, скидаємо стан утримання
        holdState->isHeld = false;
        holdState->accumulatedTime = 0.0;
    }

    #undef INCREMENT_VAL
    #undef DECREMENT_VAL

    return changed;
}

// Функція малювання слайдера з рівнем заповнення normPos (0..1)
static void DrawSlider(Rectangle bounds, float normPos, Color baseColor) {
    DrawRectangleRec(bounds, Fade(baseColor, 0.25f)); // Фон слайдера (прозорий)
    DrawRectangle(bounds.x, bounds.y, normPos * bounds.width, bounds.height, baseColor); // Заповнення
    DrawRectangleLinesEx(bounds, 1, GetContrastingColor(baseColor)); // Рамка

    // Ручка слайдера
    float knobX = bounds.x + normPos * bounds.width;
    float knobW = 12, knobH = bounds.height;
    Rectangle knobRect = {knobX - knobW / 2, bounds.y, knobW, knobH};
    DrawRectangleRec(knobRect, GetContrastingColor(baseColor));
    DrawRectangleLinesEx(knobRect, 1, baseColor);
}

// Оновлення значення за положенням миші щодо bounds слайдера (нормалізує і обмежує),
// з урахуванням кроку step для дискретизації
static void UpdateSliderValue(Rectangle bounds, void* value, void* minVal, void* maxVal,
                              float step, GuiSpinnerValueType valueType)
{
    Vector2 mousePos = GetMousePosition();
    float norm = (mousePos.x - bounds.x) / bounds.width;
    if (norm < 0) norm = 0;
    if (norm > 1) norm = 1;

    if (valueType == GUI_SPINNER_FLOAT) {
        float minV = *(float*)minVal;
        float maxV = *(float*)maxVal;
        float range = maxV - minV;
        float rawVal = minV + norm * range;

        // Використання roundf для точного округлення до кратного кроку
        float steppedVal = minV + step * roundf((rawVal - minV) / step);

        // Обмеження в межах [minV, maxV]
        if (steppedVal < minV) steppedVal = minV;
        if (steppedVal > maxV) steppedVal = maxV;

        *(float*)value = steppedVal;
    } else {
        int minV = *(int*)minVal;
        int maxV = *(int*)maxVal;
        int range = maxV - minV;

        // Розрахунок цілочисельного значення з кроком
        int rawVal = minV + (int)(norm * range);

        int stepInt = (int)(step + 0.5f);
        int steppedVal = minV + ((rawVal - minV + stepInt/2) / stepInt) * stepInt; // округлення до кроку

        // Обмеження у межах [minV, maxV]
        if (steppedVal < minV) steppedVal = minV;
        if (steppedVal > maxV) steppedVal = maxV;

        *(int*)value = steppedVal;
    }
}

// Структура для збереження стану окремого слайдера
typedef struct {
    Rectangle bounds; // Позиція слайдера на екрані
    bool isActive;    // Чи активно (утримується) перетягування слайдера
    bool used;        // Чи використовується цей елемент
} SliderState;

static SliderState slidersState[MAX_SPINNERS] = {{0}};

// Пошук або ініціалізація стану слайдера за прямокутником bounds
static bool* GetSliderActiveState(Rectangle bounds) {
    for (int i = 0; i < MAX_SPINNERS; i++) {
        if (slidersState[i].used) {
            if (memcmp(&slidersState[i].bounds, &bounds, sizeof(Rectangle)) == 0) {
                return &slidersState[i].isActive;
            }
        } else {
            slidersState[i].bounds = bounds;
            slidersState[i].isActive = false;
            slidersState[i].used = true;
            return &slidersState[i].isActive;
        }
    }
    return NULL; // Перевищено максимальну кількість слайдерів
}

bool Gui_Spinner(int id, int centerX, int centerY, int width, int height,
                 const char* textLeft, const char* textRight,
                 void* value, void* minValue, void* maxValue,
                 float step, GuiSpinnerValueType valueType,
                 Color baseColor, PSF_Font font, int spacing)
{
    if (id < 0 || id >= MAX_SPINNERS || !value) return false;

    HoldState* holdLeft = &holdLeftStates[id];
    HoldState* holdRight = &holdRightStates[id];

    bool changed = false;
    int btnSize = height;
    int sliderWidth = width - 2 * btnSize;
    int posY = centerY - height / 2;

    Rectangle leftBtn = {(float)(centerX - width / 2), (float)posY, (float)btnSize, (float)height};
    Rectangle rightBtn = {(float)(centerX + width / 2 - btnSize), (float)posY, (float)btnSize, (float)height};
    Rectangle sliderRect = {(float)(leftBtn.x + btnSize), (float)posY, (float)sliderWidth, (float)height};

    // Обробка кліків по кнопках зі стрілками
    if (ArrowButton(leftBtn, ARROW_LEFT, value, minValue, maxValue,
                    step, valueType, holdLeft, baseColor)) changed = true;
    if (ArrowButton(rightBtn, ARROW_RIGHT, value, minValue, maxValue,
                    step, valueType, holdRight, baseColor)) changed = true;

    // Визначення нормалізованого значення (0..1) для відображення слайдера
    float normVal = 0.0f;
    if (valueType == GUI_SPINNER_FLOAT) {
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

    DrawSlider(sliderRect, normVal, baseColor);

    // Отримуємо внутрішній стан "перетягування" для слайдера
    bool *isActive = GetSliderActiveState(sliderRect);
    if (!isActive) return false;

    Vector2 mousePos = GetMousePosition();
    bool mouseOver = CheckCollisionPointRec(mousePos, sliderRect);

    // Зміна значення колесом миші при наведенні на слайдер
    if (mouseOver) {
        int wheelMove = GetMouseWheelMove();
        if (wheelMove != 0) {
            if (valueType == GUI_SPINNER_FLOAT) {
                float *v = (float*)value;
                float maxV = *(float*)maxValue;
                float minV = *(float*)minValue;
                *v += step * wheelMove;
                if (*v > maxV) *v = maxV;
                if (*v < minV) *v = minV;
                changed = true;
            } else {
                int *v = (int*)value;
                int maxV = *(int*)maxValue;
                int minV = *(int*)minValue;
                *v += (int)(step) * wheelMove;
                if (*v > maxV) *v = maxV;
                if (*v < minV) *v = minV;
                changed = true;
            }
        }
    }

    // Початок перетягування слайдера
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && mouseOver) {
        *isActive = true;
    }
    // Кінець перетягування
    if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
        *isActive = false;
    }

    // Якщо слайдер тягнеться — оновлюємо значення
    if (*isActive) {
        // Запам'ятовуємо старе значення, щоб визначити чи змінювалося
        bool valueChanged = false;
        if (valueType == GUI_SPINNER_FLOAT) {
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

    // Малюємо тексти над кнопками (за наявності)
    int pad = 6;

    if (textLeft && textLeft[0]) {
        int len = 0;
        for (const char *p = textLeft; *p; p++) if (((*p)&0xC0)!=0x80) len++;   // Правильний підрахунок codepoints UTF8

        float textWidth = len * (font.width + spacing) - spacing;
        float boxW = textWidth + 2*pad;
        float boxH = font.height + 2*pad;

        Rectangle rLeft = {
            leftBtn.x + leftBtn.width/2.0f - boxW/2.0f,
            leftBtn.y - boxH - 8,
            boxW,
            boxH
        };
        Color bg = Fade(GetContrastingColor(baseColor), 0.9f);
        Color fg = GetContrastingColor(bg);
        DrawRectangleRec(rLeft, bg);
        DrawRectangleLinesEx(rLeft, 1, fg);
        DrawPSFTextScaled(font, (int)(rLeft.x + pad), (int)(rLeft.y + pad/2), textLeft, spacing, 1, fg);
    }

    if (textRight && textRight[0]) {
        int len = 0;
        for (const char *p = textRight; *p; p++) if (((*p)&0xC0)!=0x80) len++;

        float textWidth = len * (font.width + spacing) - spacing;
        float boxW = textWidth + 2*pad;
        float boxH = font.height + 2*pad;

        Rectangle rRight = {
            rightBtn.x + rightBtn.width/2.0f - boxW/2.0f,
            rightBtn.y - boxH - 8,
            boxW,
            boxH
        };
        Color bg = Fade(GetContrastingColor(baseColor), 0.9f);
        Color fg = GetContrastingColor(bg);
        DrawRectangleRec(rRight, bg);
        DrawRectangleLinesEx(rRight, 1, fg);
        DrawPSFTextScaled(font, (int)(rRight.x + pad), (int)(rRight.y + pad/2), textRight, spacing, 1, fg);
    }

    // Відображення текстового рядка з поточним значенням поруч зі слайдером
    char valStr[32];
    if(valueType == GUI_SPINNER_FLOAT)
        snprintf(valStr, sizeof(valStr), "%.2f", *(float*)value);
    else
        snprintf(valStr, sizeof(valStr), "%d", *(int*)value);

    // Обчислюємо ширину і висоту тексту у пікселях
    int textLen = 0;
    for(const char* p = valStr; *p; p++) if(((*p) & 0xC0) != 0x80) textLen++;
    int textWidth = textLen * (font.width + spacing) - spacing;
    int textHeight = font.height;

    // Центр слайдера по X та Y
    centerX = sliderRect.x + sliderRect.width / 2.0f;
    centerY = sliderRect.y + sliderRect.height / 2.0f;

    // Відступи навколо тексту в прямокутнику (padding)
    const int padX = 8;
    const int padY = 4;

    // Обчислюємо координати прямокутника фону так, щоб текст лежав по центру слайдера
    Rectangle valBgRect = {
        centerX - (textWidth / 2.0f) - padX,
        centerY - (textHeight / 2.0f) - padY,
        textWidth + 2*padX,
        textHeight + 2*padY
    };

    // Колір фону з напівпрозорістю
    Color valBgColor = Fade(baseColor, 0.2f);  // 20% прозорість, можна налаштувати
    Color valFgColor = GetContrastingColor(valBgColor);

    // Малюємо фон-прямокутник
    DrawRectangleRec(valBgRect, valBgColor);
    // Малюємо рамку навколо прямокутника (контрастною)
    DrawRectangleLinesEx(valBgRect, 1, valFgColor);

    // Малюємо текст, спроєктований у центр прямокутника
    // Обчислюємо позицію тексту (початок малювання) — з урахуванням padding і щоб текст центрувався
    int textDrawX = (int)(valBgRect.x + padX);
    int textDrawY = (int)(valBgRect.y + padY);

    DrawPSFTextScaled(font, textDrawX, textDrawY, valStr, spacing, 1, valFgColor);

    return changed;
}

