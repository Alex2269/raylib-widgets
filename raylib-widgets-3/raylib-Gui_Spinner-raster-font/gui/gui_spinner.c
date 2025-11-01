// gui_spinner.c

#include "gui_spinner.h"
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <math.h>

typedef struct {
    bool isHeld;            // Чи наразі кнопка утримується
    double holdStartTime;   // Час початку утримання
    double lastUpdateTime;  // Час останнього оновлення
    double accumulatedTime; // Накопичений час для повторення кроку
} HoldState;

static HoldState holdLeftStates[MAX_SPINNERS] = {{0}};
static HoldState holdRightStates[MAX_SPINNERS] = {{0}};

// Розширений тип напрямку стрілок із підтримкою 4-х напрямків
typedef enum {
    ARROW_LEFT,
    ARROW_RIGHT,
    ARROW_UP,
    ARROW_DOWN
} ArrowDirectionExtended;

// Інкремент значення з урахуванням меж та типу (int/float)
static inline bool IncrementValue(void* value, void* minVal, void* maxVal,
                                  float step, GuiSpinnerValueType valueType);
// Декремент значення з урахуванням меж та типу (int/float)
static inline bool DecrementValue(void* value, void* minVal, void* maxVal,
                                  float step, GuiSpinnerValueType valueType);
// Кнопка зі стрілкою з логікою натискання та утримання
static inline bool ArrowButton(Rectangle bounds, ArrowDirectionExtended dir,
                        void* value, void* minVal, void* maxVal,
                        float step, GuiSpinnerValueType valueType,
                        HoldState* holdState, Color baseColor,
                        GuiSpinnerOrientation orientation);

static double GetSystemTime() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (double)ts.tv_sec + ts.tv_nsec / 1e9;
}

static Color GetContrastingColor(Color c) {
    float luminance = 0.2126f * c.r / 255 + 0.7152f * c.g / 255 + 0.0722f * c.b / 255;
    return (luminance > 0.5f) ? BLACK : WHITE;
}

// Функція малювання трикутної стрілки в кнопці з урахуванням напряму
static void DrawArrow(Rectangle bounds, ArrowDirectionExtended dir, Color color) {
    Vector2 center = { bounds.x + bounds.width/2.0f, bounds.y + bounds.height/2.0f };
    float size = bounds.width * 0.45f;

    Vector2 points[3];
    switch (dir) {
        case ARROW_LEFT:
            points[0] = (Vector2){center.x + size, center.y - size}; // права верхня
            points[1] = (Vector2){center.x - size, center.y};        // лівий центр
            points[2] = (Vector2){center.x + size, center.y + size}; // права нижня
            break;
        case ARROW_RIGHT:
            points[1] = (Vector2){center.x - size, center.y - size}; // ліва верхня
            points[0] = (Vector2){center.x + size, center.y};        // права середина
            points[2] = (Vector2){center.x - size, center.y + size}; // ліва нижня
            break;
        case ARROW_UP:
            points[1] = (Vector2){center.x, center.y + size};       // нижня вершина
            points[0] = (Vector2){center.x - size, center.y - size}; // ліва вершина
            points[2] = (Vector2){center.x + size, center.y - size}; // права вершина
            break;
        case ARROW_DOWN:
            points[0] = (Vector2){center.x, center.y - size};       // верхня вершина
            points[1] = (Vector2){center.x - size, center.y + size}; // ліва нижня
            points[2] = (Vector2){center.x + size, center.y + size}; // права нижня
            break;
    }
    DrawTriangle(points[0], points[1], points[2], color);
}

// ====================================================================================
// Допоміжні inline-функції для інкременту/декременту значення
// ====================================================================================

// Інкрементує значення на step, з урахуванням типу і максимального значення.
// Повертає true, якщо значення змінилося.
static inline bool IncrementValue(void* value, void* minVal, void* maxVal,
                                  float step, GuiSpinnerValueType valueType)
{
    bool changed = false;

    if (valueType == GUI_SPINNER_FLOAT) {
        float* v = (float*)value;
        float maxV = *(float*)maxVal;
        // Перевірка, чи не перевищує нове значення максимум
        if (*v + step <= maxV) {
            *v += step;
            changed = true;
        } else if (*v != maxV) { // Якщо вже за межами, але не дорівнює maxV, округлюємо до maxV
            *v = maxV;
            changed = true;
        }
    }
    else {
        int* v = (int*)value;
        int maxV = *(int*)maxVal;
        int stepInt = (int)(step + 0.5f); // округлення float-кроку до int
        // Перевірка, чи не перевищує нове значення максимум
        if (*v + stepInt <= maxV) {
            *v += stepInt;
            changed = true;
        } else if (*v != maxV) { // Якщо вже за межами, але не дорівнює maxV, округлюємо до maxV
            *v = maxV;
            changed = true;
        }
    }

    return changed;
}

// Декрементує значення на step, з урахуванням типу і мінімального значення.
// Повертає true, якщо значення змінилося.
static inline bool DecrementValue(void* value, void* minVal, void* maxVal,
                                  float step, GuiSpinnerValueType valueType)
{
    bool changed = false;

    if (valueType == GUI_SPINNER_FLOAT) {
        float* v = (float*)value;
        float minV = *(float*)minVal;
        // Перевірка, чи не менше нове значення мінімуму
        if (*v - step >= minV) {
            *v -= step;
            changed = true;
        } else if (*v != minV) { // Якщо вже за межами, але не дорівнює minV, округлюємо до minV
            *v = minV;
            changed = true;
        }
    }
    else {
        int* v = (int*)value;
        int minV = *(int*)minVal;
        int stepInt = (int)(step + 0.5f); // округлення float-кроку до int
        // Перевірка, чи не менше нове значення мінімуму
        if (*v - stepInt >= minV) {
            *v -= stepInt;
            changed = true;
        } else if (*v != minV) { // Якщо вже за межами, але не дорівнює minV, округлюємо до minV
            *v = minV;
            changed = true;
        }
    }

    return changed;
}

// ====================================================================================

// Кнопка зі стрілкою з логікою натискання та утримання
// Оновлює *value залежно від натискання та типу значення
static bool ArrowButton(Rectangle bounds, ArrowDirectionExtended dir,
                        void* value, void* minVal, void* maxVal,
                        float step, GuiSpinnerValueType valueType,
                        HoldState* holdState, Color baseColor,
                        GuiSpinnerOrientation orientation)
{
    Vector2 mousePos = GetMousePosition();
    bool mouseOver = CheckCollisionPointRec(mousePos, bounds);
    bool changed = false;

    Color btnColor = baseColor;
    if (mouseOver) btnColor = Fade(baseColor, 0.8f);
    if (mouseOver && IsMouseButtonDown(MOUSE_LEFT_BUTTON)) btnColor = Fade(baseColor, 0.6f);

    Color borderColor = GetContrastingColor(btnColor);
    const int borderThickness = 2;

    DrawRectangleLinesEx((Rectangle){bounds.x - borderThickness, bounds.y - borderThickness,
        bounds.width + 2 * borderThickness, bounds.height + 2 * borderThickness},
        borderThickness, borderColor);
    DrawRectangleRec(bounds, btnColor);

    // Визначення напрямку стрілки залежно від орієнтації та кнопки
    ArrowDirectionExtended dirToDraw;

    if (orientation == GUI_SPINNER_HORIZONTAL) {
        dirToDraw = (dir == ARROW_LEFT) ? ARROW_LEFT : ARROW_RIGHT;
    } else { // вертикальна
        dirToDraw = (dir == ARROW_LEFT) ? ARROW_UP : ARROW_DOWN;
    }

    DrawArrow(bounds, dirToDraw, GetContrastingColor(btnColor));

    double now = GetSystemTime();

    if (!holdState->isHeld) {
        holdState->lastUpdateTime = now;
        holdState->accumulatedTime = 0.0;
    }

    double deltaTime = now - holdState->lastUpdateTime;
    holdState->lastUpdateTime = now;

    bool mousePressed = mouseOver && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
    bool mouseDown = mouseOver && IsMouseButtonDown(MOUSE_LEFT_BUTTON);

    const double delayBeforeAccel = 0.175;
    const double baseInterval = 0.25;
    const double minInterval = 0.005;
    const double accelRate = 0.075;

    if (mousePressed) {
        holdState->isHeld = true;
        holdState->holdStartTime = now;
        holdState->accumulatedTime = 0.0;
        if (dir == ARROW_RIGHT)
            changed = IncrementValue(value, minVal, maxVal, step, valueType);
        else
            changed = DecrementValue(value, minVal, maxVal, step, valueType);
    }
    else if (mouseDown && holdState->isHeld) {
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
                changed = IncrementValue(value, minVal, maxVal, step, valueType) || changed;
            else
                changed = DecrementValue(value, minVal, maxVal, step, valueType) || changed;
        }
    } else {
        holdState->isHeld = false;
        holdState->accumulatedTime = 0.0;
    }

    return changed;
}

static void DrawSpinner(Rectangle bounds, float normPos, Color baseColor, GuiSpinnerOrientation orientation) {
    if (orientation == GUI_SPINNER_HORIZONTAL) {
        DrawRectangleRec(bounds, Fade(baseColor, 0.25f));
        DrawRectangle(bounds.x, bounds.y, normPos * bounds.width, bounds.height, Fade(baseColor, 0.5f));
        DrawRectangleLinesEx(bounds, 1, GetContrastingColor(baseColor));

        float knobX = bounds.x + normPos * bounds.width;
        float knobW = 10, knobH = bounds.height;
        Rectangle knobRect = {knobX - knobW / 2, bounds.y, knobW, knobH};
        DrawRectangleRec(knobRect, GetContrastingColor(baseColor));
        DrawRectangleLinesEx(knobRect, 1, baseColor);
    } else {
        DrawRectangleRec(bounds, Fade(baseColor, 0.25f));
        float fillHeight = normPos * bounds.height;
        DrawRectangle(bounds.x, bounds.y + bounds.height - fillHeight, bounds.width, fillHeight, Fade(baseColor, 0.5f));

        float knobY = bounds.y + bounds.height - fillHeight;
        float knobW = bounds.width;
        float knobH = 10;
        Rectangle knobRect = {bounds.x, knobY - knobH / 2, knobW, knobH};
        DrawRectangleRec(knobRect, GetContrastingColor(baseColor));
        DrawRectangleLinesEx(knobRect, 1, baseColor);
    }
}

static void UpdateSpinnerValue(Rectangle bounds, void* value, void* minVal, void* maxVal,
                               float step, GuiSpinnerValueType valueType, GuiSpinnerOrientation orientation)
{
    Vector2 mousePos = GetMousePosition();

    float norm = 0.0f;
    if (orientation == GUI_SPINNER_HORIZONTAL) {
        norm = (mousePos.x - bounds.x) / bounds.width;
    } else {
        norm = 1.0f - (mousePos.y - bounds.y) / bounds.height;
    }

    if (norm < 0) norm = 0;
    if (norm > 1) norm = 1;

    if (valueType == GUI_SPINNER_FLOAT) {
        float minV = *(float*)minVal;
        float maxV = *(float*)maxVal;
        float range = maxV - minV;
        float rawVal = minV + norm * range;
        float steppedVal = minV + step * roundf((rawVal - minV) / step);
        if (steppedVal < minV) steppedVal = minV;
        if (steppedVal > maxV) steppedVal = maxV;
        *(float*)value = steppedVal;
    } else {
        int minV = *(int*)minVal;
        int maxV = *(int*)maxVal;
        int range = maxV - minV;
        int rawVal = minV + (int)(norm * range);
        int stepInt = (int)(step + 0.5f);
        if (stepInt == 0) stepInt = 1;
        int steppedVal = minV + ((rawVal - minV + stepInt / 2) / stepInt) * stepInt;
        if (steppedVal < minV) steppedVal = minV;
        if (steppedVal > maxV) steppedVal = maxV;
        *(int*)value = steppedVal;
    }
}

typedef struct {
    Rectangle bounds;
    bool isActive;
    bool used;
} SpinnerState;

static SpinnerState spinnersState[MAX_SPINNERS] = {{0}};

static bool* GetSpinnerActiveState(Rectangle bounds) {
    for (int i = 0; i < MAX_SPINNERS; i++) {
        if (spinnersState[i].used) {
            if (memcmp(&spinnersState[i].bounds, &bounds, sizeof(Rectangle)) == 0)
                return &spinnersState[i].isActive;
        } else {
            spinnersState[i].bounds = bounds;
            spinnersState[i].isActive = false;
            spinnersState[i].used = true;
            return &spinnersState[i].isActive;
        }
    }
    return NULL;
}

// Головна функція спінера з орієнтацією
bool Gui_Spinner(int id, int centerX, int centerY, int width, int height,
                 const char* textLeft, const char* textRight,
                 void* value, void* minValue, void* maxValue,
                 float step, GuiSpinnerValueType valueType,
                 GuiSpinnerOrientation orientation,
                 Color baseColor, RasterFont font, int spacing)
{
    if (id < 0 || id >= MAX_SPINNERS || !value) return false;

    HoldState* holdLeft = &holdLeftStates[id];
    HoldState* holdRight = &holdRightStates[id];

    bool changed = false;

    int btnSize = (orientation == GUI_SPINNER_HORIZONTAL) ? height : width;

    int spinnerWidth = (orientation == GUI_SPINNER_HORIZONTAL) ? (width - 2 * btnSize) : width;
    int spinnerHeight = (orientation == GUI_SPINNER_HORIZONTAL) ? height : (height - 2 * btnSize);

    int posX = centerX - width / 2;
    int posY = centerY - height / 2;

    Rectangle leftBtn, rightBtn, spinnerRect;

    if (orientation == GUI_SPINNER_HORIZONTAL) {
        // Горизонтальна орієнтація: звична логіка
        leftBtn = (Rectangle){(float)posX, (float)posY, (float)btnSize, (float)height};
        rightBtn = (Rectangle){(float)(posX + width - btnSize), (float)posY, (float)btnSize, (float)height};
        spinnerRect = (Rectangle){(float)(posX + btnSize), (float)posY, (float)spinnerWidth, (float)spinnerHeight};

        if (ArrowButton(leftBtn, ARROW_LEFT, value, minValue, maxValue, step, valueType, holdLeft, baseColor, orientation))
            changed = true;
        if (ArrowButton(rightBtn, ARROW_RIGHT, value, minValue, maxValue, step, valueType, holdRight, baseColor, orientation))
            changed = true;
    } else {
        // Вертикальна орієнтація: обернути інкремент і декремент
        leftBtn = (Rectangle){(float)posX, (float)posY, (float)width, (float)btnSize};              // ВЕРХНЯ кнопка
        rightBtn = (Rectangle){(float)posX, (float)(posY + height - btnSize), (float)width, (float)btnSize};  // НИЖНЯ кнопка
        spinnerRect = (Rectangle){(float)posX, (float)(posY + btnSize), (float)spinnerWidth, (float)spinnerHeight};

        // Верхня кнопка — ІНКРЕМЕНТ (стрілка вгору)
        if (ArrowButton(leftBtn, ARROW_RIGHT, value, minValue, maxValue, step, valueType, holdLeft, baseColor, orientation))
            changed = true;

        // Нижня кнопка — ДЕКРЕМЕНТ (стрілка вниз)
        if (ArrowButton(rightBtn, ARROW_LEFT, value, minValue, maxValue, step, valueType, holdRight, baseColor, orientation))
            changed = true;
    }

    float normVal;
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

    DrawSpinner(spinnerRect, normVal, baseColor, orientation);

    bool* isActive = GetSpinnerActiveState(spinnerRect);
    if (!isActive) return false;

    Vector2 mousePos = GetMousePosition();
    bool mouseOver = CheckCollisionPointRec(mousePos, spinnerRect);

    if (mouseOver) {
        int wheelMove = GetMouseWheelMove();
        if (wheelMove != 0) {
            float delta = step * wheelMove;
            if (valueType == GUI_SPINNER_FLOAT) {
                float* v = (float*)value;
                float maxV = *(float*)maxValue;
                float minV = *(float*)minValue;
                *v += delta;
                if (*v > maxV) *v = maxV;
                if (*v < minV) *v = minV;
            } else {
                int* v = (int*)value;
                int maxV = *(int*)maxValue;
                int minV = *(int*)minValue;
                int stepInt = (int)(step + 0.5f);
                int deltaInt = (int)(stepInt * wheelMove);
                *v += deltaInt;
                if (*v > maxV) *v = maxV;
                if (*v < minV) *v = minV;
            }
            changed = true;
        }
    }

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && mouseOver)
        *isActive = true;

    if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
        *isActive = false;

    if (*isActive) {
        bool valueChanged = false;
        if (valueType == GUI_SPINNER_FLOAT) {
            float oldVal = *(float*)value;
            UpdateSpinnerValue(spinnerRect, value, minValue, maxValue, step, valueType, orientation);
            if (*(float*)value != oldVal) valueChanged = true;
        } else {
            int oldVal = *(int*)value;
            UpdateSpinnerValue(spinnerRect, value, minValue, maxValue, step, valueType, orientation);
            if (*(int*)value != oldVal) valueChanged = true;
        }
        if (valueChanged) changed = true;
    }


    // ==============================================================================
    // Малювання текстів над кнопками (якщо вони задані)
    // ==============================================================================
    int pad = 6; // Відступ для тексту всередині рамки

    // Текст над лівою кнопкою
    if (textLeft && textLeft[0]) {
        // Підрахунок кількості кодових точок UTF-8 для правильного виміру ширини тексту
        int len = 0;
        for (const char *p = textLeft; *p; p++) if (((*p)&0xC0)!=0x80) len++;

        float textWidth = len * (font.glyph_width + spacing) - spacing;
        float boxW = textWidth + 2*pad;
        float boxH = font.glyph_height + 2*pad;

        // Позиція рамки з текстом над лівою кнопкою
        Rectangle rLeft = {
            leftBtn.x + leftBtn.width/2.0f - boxW/2.0f,
            leftBtn.y - boxH - 8, // Відступ вгору від кнопки
            boxW,
            boxH
        };
        Color bg = Fade(GetContrastingColor(baseColor), 0.9f); // Фон рамки
        Color fg = GetContrastingColor(bg); // Колір тексту
        DrawRectangleRec(rLeft, bg);
        DrawRectangleLinesEx(rLeft, 1, fg);
        DrawTextScaled(font, (int)(rLeft.x + pad), (int)(rLeft.y + pad/2), textLeft, spacing, 1, fg);
    }

    // Текст над правою кнопкою
    if (textRight && textRight[0]) {
        // Підрахунок кількості кодових точок UTF-8
        int len = 0;
        for (const char *p = textRight; *p; p++) if (((*p)&0xC0)!=0x80) len++;

        float textWidth = len * (font.glyph_width + spacing) - spacing;
        float boxW = textWidth + 2*pad;
        float boxH = font.glyph_height + 2*pad;

        // Позиція рамки з текстом над правою кнопкою
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
        DrawTextScaled(font, (int)(rRight.x + pad), (int)(rRight.y + pad/2), textRight, spacing, 1, fg);
    }

    // ==============================================================================
    // Відображення текстового рядка з поточним значенням у центрі слайдера
    // ==============================================================================
    char valStr[32]; // Буфер для форматованого значення
    if(valueType == GUI_SPINNER_FLOAT)
        snprintf(valStr, sizeof(valStr), "%.2f", *(float*)value); // Форматування float
        else
            snprintf(valStr, sizeof(valStr), "%d", *(int*)value);     // Форматування int

            // Обчислюємо ширину і висоту тексту у пікселях для центрування
            int textLen = 0;
        for(const char* p = valStr; *p; p++) if(((*p) & 0xC0) != 0x80) textLen++;
        int textWidth = textLen * (font.glyph_width + spacing) - spacing;
    int textHeight = font.glyph_height;

    // Центр слайдера по X та Y для позиціонування тексту
    centerX = spinnerRect.x + spinnerRect.width / 2.0f;
    centerY = spinnerRect.y + spinnerRect.height / 2.0f;

    // Відступи навколо тексту в прямокутнику фону (padding)
    const int padX = 8;
    const int padY = 4;

    // Обчислюємо координати прямокутника фону так, щоб текст лежав по центру слайдера
    Rectangle valBgRect = {
        centerX - (textWidth / 2.0f) - padX,
        centerY - (textHeight / 2.0f) - padY,
        textWidth + 2*padX,
        textHeight + 2*padY
    };

    // Колір фону для відображення значення з напівпрозорістю
    Color valBgColor = Fade(baseColor, 0.2f);
    Color valFgColor = GetContrastingColor(valBgColor); // Контрастний колір для тексту

    // Малюємо фон-прямокутник
    // DrawRectangleRec(valBgRect, valBgColor);
    // Малюємо рамку навколо прямокутника (контрастною)
    // DrawRectangleLinesEx(valBgRect, 1, valFgColor);

    // Малюємо текст, спроєктований у центр прямокутника
    int textDrawX = (int)(valBgRect.x + padX);
    int textDrawY = (int)(valBgRect.y + padY);

    DrawTextScaled(font, textDrawX, textDrawY, valStr, spacing, 1, valFgColor);

    return changed; // Повертаємо true, якщо значення спінера змінилося
}



