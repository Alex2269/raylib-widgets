#include "raylib.h"
#include "psf_font.h"
#include "slider_widget.h"
#include <stdio.h>

PSF_Font font18;

// Глобальні змінні для значень слайдерів
float sliderValue1 = 50.0f;
float sliderValue2 = 75.0f;
float sliderValue3 = 25.0f;
float sliderValue4 = 15.0f;

void UpdateDrawFrame(void) {
    // Оновлюємо та реєструємо стан слайдерів
    RegisterSlider(0, (Rectangle){ 100, 100, 10, 300 }, &sliderValue1, 0.0f, 100.0f, true, RED, "Cursor 1", "Value:");
    RegisterSlider(1, (Rectangle){ 100, 100, 10, 300 }, &sliderValue2, 0.0f, 100.0f, true, GREEN, "Cursor 2", "Value:");
    RegisterSlider(2, (Rectangle){ 100, 100, 10, 300 }, &sliderValue3, 0.0f, 100.0f, true, YELLOW, "Cursor 3", "Value:");
    RegisterSlider(3, (Rectangle){ 100, 100, 10, 300 }, &sliderValue4, 0.0f, 100.0f, true, GRAY, "Cursor 3", "Value:");

    BeginDrawing();
    ClearBackground(GRAY);

    // Централізована функція, яка обробляє взаємодію і малює всі слайдери
    UpdateSlidersAndDraw(font18, 2);

    EndDrawing();
}

int main(void) {
    InitWindow(800, 600, "Slider Widget with Overlapping Cursors");
    font18 = LoadPSFFont("fonts/Uni3-TerminusBold18x10.psf");

    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        UpdateDrawFrame();
    }

    UnloadPSFFont(font18);
    CloseWindow();
    return 0;
}
