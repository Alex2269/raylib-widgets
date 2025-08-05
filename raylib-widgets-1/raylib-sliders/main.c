#include "raylib.h"
#include "sliders.h"


int main(void)
{
    // Ініціалізація вікна
    const int screenWidth = 800;
    const int screenHeight = 600;
    InitWindow(screenWidth, screenHeight, "Raylib Button Example");

    // Головний цикл програми
    while (!WindowShouldClose())
    {
        /* Виклик обробки повзунків */
        update_sliders();

        // Початок малювання
        BeginDrawing();

        ClearBackground(BLACK);


        /* Малюємо повзунки */
        draw_sliders();

        EndDrawing();
    }

    // Закриття вікна
    CloseWindow();

    return 0;
}
