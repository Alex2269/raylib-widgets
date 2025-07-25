#include "raylib.h"
#include "psf_font.h"
#include "gui_spinner.h"
#include <stdio.h>

int spacing = 2;
PSF_Font font18;

// Приклади двох значень: ціле і з рухомою комою
int intValue = 10;
float floatValue = 25.5f;

// Мінімальні і максимальні значення для int і float спінерів
int intMin = 0, intMax = 100;
float floatMin = 0.0f, floatMax = 100.0f;

int main(void) {
    InitWindow(600, 350, "Gui Spinner example (int and float)");

    SetTargetFPS(60);

    font18 = LoadPSFFont("fonts/Uni3-TerminusBold18x10.psf");

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        // Спінер для int (крок 1)
        bool changedInt = Gui_Spinner(0, 300, 100, 320, 40, "Менше", "Більше",
                                     &intValue, &intMin, &intMax,
                                     1, GUI_SPINNER_INT, BLUE, font18, spacing);

        // Спінер для float (крок 0.25)
        bool changedFloat = Gui_Spinner(1, 300, 200, 320, 40, "Менше", "Більше",
                                       &floatValue, &floatMin, &floatMax,
                                       0.25f, GUI_SPINNER_FLOAT, GREEN, font18, spacing);

        // Відображення інформації вгорі
        char text[64];
        snprintf(text, sizeof(text), "intValue: %d  floatValue: %.2f", intValue, floatValue);
        DrawPSFText(font18, 20, 20, text, spacing, BLACK);

        EndDrawing();
    }

    UnloadPSFFont(font18);
    CloseWindow();
    return 0;
}

