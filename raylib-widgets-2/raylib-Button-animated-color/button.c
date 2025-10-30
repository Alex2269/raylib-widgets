// button.c

#include "raylib.h"
#include "button.h"

// typedef struct {
//     Rectangle bounds;
//     bool pressed;
//     float animProgress; // 0..1
// } AnimatedButton;
//
// Color ColorLerp(Color c1, Color c2, float t)
// {
//     Color result = {
//         (unsigned char)(c1.r + (c2.r - c1.r) * t),
//         (unsigned char)(c1.g + (c2.g - c1.g) * t),
//         (unsigned char)(c1.b + (c2.b - c1.b) * t),
//         (unsigned char)(c1.a + (c2.a - c1.a) * t)
//     };
//     return result;
// }

void UpdateButton(AnimatedButton *btn)
{
    Vector2 mouse = GetMousePosition();
    bool mouseOver = CheckCollisionPointRec(mouse, btn->bounds);

    if (mouseOver && IsMouseButtonDown(MOUSE_LEFT_BUTTON))
        btn->pressed = true;

    if (btn->pressed && IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
        btn->pressed = false;

    // Плавно змінюємо animProgress
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
}

void DrawButton(AnimatedButton *btn, const char *text, Font font, int fontSize, int lineSpacing, Color colorNormal, Color colorPressed)
{
    Color currentColor = ColorLerp(colorNormal, colorPressed, btn->animProgress);

    UpdateButton(btn);

    DrawRectangleRec(btn->bounds, currentColor);

    Vector2 textSize = MeasureTextEx(font, text, fontSize, lineSpacing);
    Vector2 textPos = {
        btn->bounds.x + (btn->bounds.width - textSize.x) / 2,
        btn->bounds.y + (btn->bounds.height - textSize.y) / 2
    };
    DrawTextEx(font, text, textPos, fontSize, lineSpacing, WHITE);
}
