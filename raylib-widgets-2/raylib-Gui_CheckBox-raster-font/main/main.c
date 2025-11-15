#include "raylib.h"
#include "guicheckbox.h"
#include "all_font.h"
#include <stddef.h>

int LineSpacing = 0;
int spacing = 2;

int main(void)
{
    const int screenWidth = 600;
    const int screenHeight = 200;
    InitWindow(screenWidth, screenHeight, "Example: Radio Buttons with Gui_CheckBox");

    SetTargetFPS(60);

    // Змінна для зберігання вибраного індексу (0, 1 або 2)
    int selected = 0;

    // Тексти для радіокнопок
    const char *options[] = {
        "Варіант 1",
        "Варіант 2",
        "Варіант 3",
    };
    const int optionsCount = 3;

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        // Малюємо радіокнопки вертикально
        for (int i = 0; i < optionsCount; i++)
        {
            bool checked = (selected == i);
            Rectangle box = { 50, 50 + i * 50, 30, 30 };

            Gui_CheckBox(box, &checked, TerminusBold24x12_font,
                         NULL, options[i], DARKGRAY);

            // Якщо чекбокс активний і натиснутий - оновлюємо selected
            // Для цього треба прийняти, що кліки змінюють checked і同步 selected
            Vector2 mouse = GetMousePosition();
            bool mouseOver = CheckCollisionPointRec(mouse, box);
            if (mouseOver && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            {
                selected = i;
            }
        }

        // Виводимо вибраний варіант текстом нижче
        DrawTextScaled(TerminusBold24x12_font, 50, 200,
                       TextFormat("Вибрано: %s", options[selected]), spacing, 2, BLACK);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}

