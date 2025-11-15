// main.c

#include "raylib.h"
#include "all_font.h" // Опис шрифтів як структури RasterFont
#include "glyphs.h"
#include "gui_slider_spinner.h"
#include <stdio.h>

int spacing = 2; // Відстань між символами

// Приклади двох значень: ціле і з рухомою комою
int iVal1 = 10;
int iVal2 = 10;
float fVal1 = 25.5f;
float fVal2 = 25.5f;
float fVal3 = 25.5f;
float fVal4 = 25.5f;

// Мінімальні і максимальні значення для int і float спінерів
int intMin = 100, intMax = -100;
float floatMin = 100.0f, floatMax = -100.0f;
bool showButtons = 1;

int main(void) {
    InitWindow(600, 350, "Gui Spinner example (int and float)");

    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(LIGHTGRAY);

        // Спінер для int (крок 1)
        Gui_SliderSpinner(0, 300, 100, 320, 40, "Менше", "Більше",
                    &fVal1, &floatMax, &floatMin,
                    0.5, GUI_SPINNER_FLOAT, GUI_SPINNER_HORIZONTAL,
                    YELLOW, TerminusBold24x12_font, spacing, showButtons);

        // Спінер для float (крок 0.25)
        Gui_SliderSpinner(1, 300, 250, 320, 40, "Менше", "Більше",
                    &fVal2, &floatMin, &floatMax,
                    0.5f, GUI_SPINNER_FLOAT, GUI_SPINNER_HORIZONTAL,
                    GREEN, TerminusBold24x12_font, spacing, showButtons);

        Gui_SliderSpinner(2, 50, 175, 20, 240, "Менше", "Більше",
                    &iVal1, &intMax, &intMin,
                    1, GUI_SPINNER_INT, GUI_SPINNER_VERTICAL,
                    BLUE, Terminus12x6_font, spacing, showButtons);

        Gui_SliderSpinner(3, 550, 175, 20, 240, "Менше", "Більше",
                    &fVal3, &floatMin, &floatMax,
                    0.5f, GUI_SPINNER_FLOAT, GUI_SPINNER_VERTICAL,
                    RED, Terminus12x6_font, spacing, showButtons);

        Gui_SliderSpinner(4, 300, 300, 320, 20, "Менше", "Більше",
                   &fVal4, &floatMin, &floatMax,
                   0.5f, GUI_SPINNER_FLOAT, GUI_SPINNER_HORIZONTAL,
                   RED, Terminus12x6_font, spacing, !showButtons);

        // Відображення інформації вгорі
        char text1[64];
        char text2[64];
        char text3[64];
        snprintf(text1, sizeof(text1), "iVal1: %d", iVal1);
        snprintf(text2, sizeof(text2), "fVal1: %.2f\nfVal2: %.2f", fVal1, fVal2);
        snprintf(text3, sizeof(text3), "fVal3: %.2f\nfVal4: %.2f", fVal3, fVal4);

        DrawTextScaled(TerminusBold18x10_font, 30, 10, text1, spacing, 1, BLACK);
        DrawTextScaled(TerminusBold18x10_font, 120, 140, text2, spacing, 1, BLACK);
        DrawTextScaled(TerminusBold18x10_font, 350, 140, text3, spacing, 1, BLACK);

        EndDrawing();
    }

    // Після виходу з циклу звільняємо пам'ять шрифту

    CloseWindow();

    return 0;
}


