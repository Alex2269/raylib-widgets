#include "guicheckbox.h"
#include "raylib.h"

extern int fontSize;
extern int LineSpacing;
extern Font font;

void Gui_CheckBox(Rectangle bounds, bool *checked, const char *text)
{
    Vector2 mousePoint = GetMousePosition();

    // Малюємо чекбокс
    Color boxColor = (*checked) ? GREEN : LIGHTGRAY;
    DrawRectangleRec(bounds, boxColor);
    // DrawText(text, (int)(bounds.x + bounds.width + 10), (int)(bounds.y), 20, WHITE);
    DrawTextEx(font, text,
               (Vector2){(int)(bounds.x + bounds.width + 10), (int)(bounds.y),
                20, 20}, fontSize, LineSpacing, BLACK);

    // Обробляємо натискання миші
    if (CheckCollisionPointRec(mousePoint, bounds))
    {
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            *checked = !(*checked);
        }
    }
}

