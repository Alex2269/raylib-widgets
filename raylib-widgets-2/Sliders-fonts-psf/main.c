// main.c

#include "raylib.h"
#include "LoadFontUnicode.h"
#include "psf_font.h"
// #include "GlyphCache.h"
#include "sliders.h"
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
    Rectangle slider_horisontal = { 75, 50, sliderWidth, sliderHeight };
    Gui_Slider(slider_horisontal, font18, "Vertical", "Vertical", &sliderValue1, 0.0f, 100.0f, true, DARKGRAY);
    // DrawText(TextFormat("Slider Value: %.2f", sliderValue1), 100, 100, 20, DARKGRAY);

    Rectangle slider_vertical = {200, 250, 300, 20};
    Gui_Slider(slider_vertical, font18, "Horisontal", NULL, &sliderValue2, 0.0f, 100.0f, false, DARKGRAY);
    // DrawText(TextFormat("Slider Value: %.2f", sliderValue2), 100, 200, 20, DARKGRAY);

    EndDrawing();
}

int main(void)
{
    // Ініціалізація вікна
    const int screenWidth = 800;
    const int screenHeight = 600;

    // Завантаження PSF шрифту (шлях до вашого файлу)
    font18 = LoadPSFFont("fonts/Uni3-TerminusBold18x10.psf");
    font32 = LoadPSFFont("fonts/Uni3-TerminusBold32x16.psf");

    InitWindow(screenWidth, screenHeight, "Raylib Button Example");

    // Головний цикл програми
    while (!WindowShouldClose())
    {

        UpdateDrawFrame();
    }

    // Після виходу з циклу звільняємо пам'ять шрифту
    // GlyphCache_ClearAllCaches();
    UnloadPSFFont(font32);
    UnloadPSFFont(font18);

    // Закриття вікна
    CloseWindow();

    return 0;
}
