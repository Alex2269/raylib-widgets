// main.c

#include "raylib.h"
#include "all_font.h" // Опис шрифтів як структури RasterFont
#include "glyphs.h"
#include "gui_slider.h"
#include <stdio.h>


int spacing = 2;

// Приклади двох значень: ціле і з рухомою комою
int intValue = 10;
float floatValue = 25.5f;

// Мінімальні і максимальні значення для int і float спінерів
int intMin = 0, intMax = 100;
float floatMin = 0.0f, floatMax = 100.0f;

int main(void) {
    InitWindow(600, 350, "Gui Spinner example (int and float)");

    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        // Спінер для int (крок 1)
        bool changedInt = Gui_Slider(0, 300, 100, 320, 40, "Менше", "Більше",
                                      &intValue, &intMin, &intMax,
                                      1, GUI_SLIDER_INT, BLUE, TerminusBold24x12_font, spacing);

        // Спінер для float (крок 0.25)
        bool changedFloat = Gui_Slider(1, 300, 200, 320, 40, "Менше", "Більше",
                                        &floatValue, &floatMin, &floatMax,
                                        0.25f, GUI_SLIDER_FLOAT, GREEN, TerminusBold24x12_font, spacing);

        // Відображення інформації вгорі
        char text[64];
        snprintf(text, sizeof(text), "intValue: %d  floatValue: %.2f", intValue, floatValue);
        DrawTextScaled(TerminusBold24x12_font, 20, 20, text, spacing, 1, BLACK);

        EndDrawing();
    }

    // Після виходу з циклу звільняємо пам'ять шрифту

    CloseWindow();

    return 0;
}


