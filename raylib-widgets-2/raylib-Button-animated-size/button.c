// button.c

#include "button.h"

// typedef struct {
//     Rectangle bounds;
//     float scale;         // масштаб кнопки (1.0 - нормальний)
//     bool pressed;
//     float animProgress;  // прогрес анімації від 0 до 1
// } AnimatedButton;

void UpdateButton(AnimatedButton *btn)
{
    Vector2 mouse = GetMousePosition();
    bool mouseOver = CheckCollisionPointRec(mouse, btn->bounds);

    if (mouseOver && IsMouseButtonDown(MOUSE_LEFT_BUTTON))
    {
        btn->pressed = true;
    }
    if (btn->pressed && IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
    {
        btn->pressed = false;
        // Тут можна обробити подію натискання
    }

    // Плавне збільшення або зменшення масштабу
    if (btn->pressed)
    {
        btn->animProgress += 0.1f;
        if (btn->animProgress > 1.0f) btn->animProgress = 1.0f;
    }
    else
    {
        btn->animProgress -= 0.1f;
        if (btn->animProgress < 0.0f) btn->animProgress = 0.0f;
    }

    // Інтерполюємо масштаб від 1.0 до 0.9 (трохи зменшуємо кнопку при натисканні)
    btn->scale = 1.0f - 0.1f * btn->animProgress;
}

void DrawButton(AnimatedButton *btn, const char *text, Font font, int fontSize, int lineSpacing)
{
    float scaledWidth = btn->bounds.width * btn->scale;
    float scaledHeight = btn->bounds.height * btn->scale;
    float offsetX = (btn->bounds.width - scaledWidth) / 2;
    float offsetY = (btn->bounds.height - scaledHeight) / 2;

    Rectangle scaledBounds = {
        btn->bounds.x + offsetX,
        btn->bounds.y + offsetY,
        scaledWidth,
        scaledHeight
    };

    UpdateButton(btn);

    Color btnColor = btn->pressed ? DARKGRAY : LIGHTGRAY;
    DrawRectangleRec(scaledBounds, btnColor);

    Vector2 textSize = MeasureTextEx(font, text, fontSize, lineSpacing);
    Vector2 textPos = {
        scaledBounds.x + (scaledBounds.width - textSize.x) / 2,
        scaledBounds.y + (scaledBounds.height - textSize.y) / 2
    };
    DrawTextEx(font, text, textPos, fontSize, lineSpacing, WHITE);
}
