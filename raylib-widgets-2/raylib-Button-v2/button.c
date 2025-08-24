#include "button.h"
#include "raylib.h"

// Зовнішні змінні, які ти вже маєш
extern Font font;
extern int fontSize;
extern int LineSpacing;

// Функція кнопки
bool Gui_Button(Rectangle bounds, const char *text,
                Color colorNormal, Color colorHover, Color colorPressed, Color colorText)
{
    Vector2 mousePoint = GetMousePosition();
    bool pressed = false;

    bool mouseOver = CheckCollisionPointRec(mousePoint, bounds);
    Color btnColor = colorNormal;

    if (mouseOver) btnColor = colorHover;
    if (mouseOver && IsMouseButtonDown(MOUSE_LEFT_BUTTON)) btnColor = colorPressed;

    DrawRectangleRec(bounds, btnColor);

    Vector2 textSize = MeasureTextEx(font, text, fontSize, LineSpacing);
    Vector2 textPos = {
        bounds.x + (bounds.width - textSize.x) / 2,
        bounds.y + (bounds.height - textSize.y) / 2
    };
    DrawTextEx(font, text, textPos, fontSize, LineSpacing, colorText);

    if (mouseOver && IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
    {
        pressed = true;
    }

    return pressed;
}

