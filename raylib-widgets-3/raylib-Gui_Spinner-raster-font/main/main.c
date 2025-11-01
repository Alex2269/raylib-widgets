// main.c

#include "raylib.h"
#include "all_font.h" // Опис шрифтів як структури RasterFont
#include "glyphs.h"
#include "gui_spinner.h"
#include <stdio.h>

int spacing = 2; // Відстань між символами

// Приклади двох значень: ціле і з рухомою комою
int iVal1 = 10;
int iVal2 = 10;
float fVal1 = 25.5f;
float fVal2 = 25.5f;

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
        Gui_Spinner(0, 300, 100, 320, 40, "Менше", "Більше",
                    &iVal1, &intMin, &intMax,
                    1, GUI_SPINNER_INT, GUI_SPINNER_HORIZONTAL,
                    BLUE, TerminusBold24x12_font, spacing);

        // Спінер для float (крок 0.25)
        Gui_Spinner(1, 300, 200, 320, 40, "Менше", "Більше",
                    &fVal1, &floatMin, &floatMax,
                    0.25f, GUI_SPINNER_FLOAT, GUI_SPINNER_HORIZONTAL,
                    GREEN, TerminusBold24x12_font, spacing);

        Gui_Spinner(2, 100, 175, 20, 240, "Менше", "Більше",
                    &iVal2, &intMin, &intMax,
                    1, GUI_SPINNER_INT, GUI_SPINNER_VERTICAL,
                    BLUE, TerminusBold24x12_font, spacing);

        Gui_Spinner(3, 500, 175, 20, 240, "Менше", "Більше",
                    &fVal2, &floatMin, &floatMax,
                    0.5f, GUI_SPINNER_FLOAT, GUI_SPINNER_VERTICAL,
                    BLUE, TerminusBold24x12_font, spacing);

        // Відображення інформації вгорі
        char text[64];
        snprintf(text, sizeof(text), "iVal1: %d\niVal2: %d\nfVal1: %.2f\nfVal2: %.2f", iVal1, iVal2, fVal1, fVal2);

        DrawTextScaled(TerminusBold24x12_font, 200, 20, text, spacing, 1, BLACK);

        EndDrawing();
    }

    // Після виходу з циклу звільняємо пам'ять шрифту

    CloseWindow();

    return 0;
}


