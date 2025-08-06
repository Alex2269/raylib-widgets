#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> // usleep
#include <stdbool.h>
#include <stdint.h>

#include "raylib.h"
#include "draw_grid.h"
#include "DrawVerticalScale.h"

#include "LoadFontUnicode.h"
#include "psf_font.h"
// #include "GlyphCache.h"

PSF_Font font12;
PSF_Font font18;
PSF_Font font20;
PSF_Font font22;
PSF_Font font24;
PSF_Font font28;
PSF_Font font32;

int fontSize = 24;
int LineSpacing = 0;
Font font;
int spacing = 2; // Відступ між символами, той самий, що передається у DrawPSFText

int main(void) {
    const int screenWidth = 1000;
    const int screenHeight = 600;

    // Встановлюємо прапорець для мультисемплінгу (покращення якості графіки)
    SetConfigFlags(FLAG_MSAA_4X_HINT);

    InitWindow(screenWidth, screenHeight, "Raylib Oscilloscope with Trigger and Scaling");

    // Завантаження PSF шрифту (шлях до файлу)
    font12 = LoadPSFFont("fonts/Uni3-Terminus12x6.psf");
    font18 = LoadPSFFont("fonts/Uni3-TerminusBold18x10.psf");
    font20 = LoadPSFFont("fonts/Uni3-TerminusBold20x10.psf");
    font22 = LoadPSFFont("fonts/Uni3-TerminusBold22x11.psf");
    font24 = LoadPSFFont("fonts/Uni3-TerminusBold24x12.psf");
    font28 = LoadPSFFont("fonts/Uni3-TerminusBold28x14.psf");
    font32 = LoadPSFFont("fonts/Uni3-TerminusBold32x16.psf");

    SetTargetFPS(60);

    float scale_y = 1;
    float offset_y = 0;

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(DARKGRAY);

        draw_grid(screenWidth, screenHeight);

        Rectangle scaleArea = { 1, 0, 50, 600};
        DrawVerticalScale(1, scale_y, offset_y, scaleArea, font12, WHITE);

        EndDrawing();
    }

    // Після виходу з циклу звільняємо пам'ять шрифту
    // GlyphCache_ClearAllCaches();

    UnloadPSFFont(font12);
    UnloadPSFFont(font18);
    UnloadPSFFont(font20);
    UnloadPSFFont(font22);
    UnloadPSFFont(font24);
    UnloadPSFFont(font28);
    UnloadPSFFont(font32);

    CloseWindow();

    return 0;
}

