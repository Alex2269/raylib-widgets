// main.c

#include "raylib.h"
#include "LoadFontUnicode.h"
#include "psf_font.h"
#include "sliders_ex.h"
#include <stdio.h>

PSF_Font font18;
PSF_Font font32;

int fontSize = 24;
int LineSpacing = 0;  // Відступ між рядками тексту, у пікселях
int spacing = 2; // Відступ між символами

float sliderValue1 = 50.0f;
float sliderValue2 = 50.0f;

void UpdateDrawFrame(void) {
    BeginDrawing();
    ClearBackground(RAYWHITE);

    int sliderWidth = 10;
    int sliderHeight = 450;
    Rectangle slider_vertical = { 75, 50, sliderWidth, sliderHeight };
    Gui_SliderEx(0,slider_vertical, font18, "Vertical", "Vertical", &sliderValue1, 0.0f, 100.0f, true, DARKGRAY);

    Rectangle slider_horizontal = { 200, 250, 300, 20 };
    Gui_SliderEx(1,slider_horizontal, font18, "Horizontal", NULL, &sliderValue2, 0.0f, 100.0f, false, DARKGRAY);

    EndDrawing();
}

int main(void)
{
    const int screenWidth = 800;
    const int screenHeight = 600;

    font18 = LoadPSFFont("fonts/Uni3-TerminusBold18x10.psf");
    font32 = LoadPSFFont("fonts/Uni3-TerminusBold32x16.psf");

    InitWindow(screenWidth, screenHeight, "Raylib Slider Example");

    while (!WindowShouldClose())
    {
        UpdateDrawFrame();
    }

    UnloadPSFFont(font32);
    UnloadPSFFont(font18);

    CloseWindow();

    return 0;
}
