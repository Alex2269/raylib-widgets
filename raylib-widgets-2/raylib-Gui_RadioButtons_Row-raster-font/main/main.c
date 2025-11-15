// main.c

#include "raylib.h"
#include "gui_radiobutton_row.h"
#include <stdio.h>

#include "all_font.h"
#include "glyphs.h"

int LineSpacing = 0;
int spacing = 2;

int main(void)
{
    const int screenWidth = 800;
    const int screenHeight = 450;

    // Встановлюємо прапорець для мультисемплінгу (покращення якості графіки)
    SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow(screenWidth, screenHeight, "Radio Buttons Example with Instant Update");
    SetTargetFPS(60);

    const char *radiobuttonItems[] = { "R", "F", "A" };
    int selectedIndex = 0; // Поточний вибір

    // радіокнопоки тригера
    Rectangle radioBounds = { 100, 75, 3 * 30 + 2 * 5, 30 }; // 3 кнопки 30px + 2 проміжки по 5px

    // Основний цикл програми, виконується поки вікно не закриють
    while (!WindowShouldClose())
    {
        BeginDrawing();          // Починаємо малювання кадру
        ClearBackground(RAYWHITE); // Очищаємо фон білим кольором

        // Відразу оновлюємо вибір тут
        selectedIndex = Gui_RadioButtons_Row(radioBounds, TerminusBold24x12_font, radiobuttonItems, 3,
                                             selectedIndex, YELLOW, 30, 5);

        DrawTextScaled(TerminusBold24x12_font, 50, 20,
                       TextFormat("Вибрано: %s", radiobuttonItems[selectedIndex]),
                       spacing, 2, BLACK);

        EndDrawing();
    }

    // Закриваємо вікно і звільняємо ресурси
    CloseWindow();

    return 0;
}

