// main.c

#include "raylib.h"
#include "psf_font.h"
#include "value_spinner.h"

// Глобальні змінні
PSF_Font font18;
int spacing = 2;

int main(void) {
    InitWindow(400, 200, "Value Spinner Widget Example");
    SetTargetFPS(60);

    int value = 0;
    ValueSpinner spinner;

    font18 = LoadPSFFont("fonts/Uni3-TerminusBold18x10.psf");

    ValueSpinner_Init(&spinner, 50, 70, 20, 20, 0, 1000, 1, &value, font18, spacing, BLUE, RED, LIGHTGRAY);

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        if (ValueSpinner_UpdateDraw(&spinner)) {
            // Значення змінилось, можна реагувати
        }

        EndDrawing();
    }

    UnloadPSFFont(font18);
    CloseWindow();
    return 0;
}

