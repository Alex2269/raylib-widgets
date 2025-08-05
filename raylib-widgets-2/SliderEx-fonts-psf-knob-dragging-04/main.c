#include "raylib.h"
#include "psf_font.h"
#include "slider_widget_triangle.h"
#include <stdio.h>

int LineSpacing = 0;  // Відступ між рядками тексту, у пікселях
PSF_Font font18;

// Глобальні змінні для значень слайдерів
float sliderValue1 = 50.0f;
float sliderValue2 = 75.0f;
float sliderValue3 = 25.0f;
float sliderValue4 = 15.0f;

void UpdateDrawFrame(void) {

    // Реєстрація слайдера з трикутною ручкою, напрямок вліво
    RegisterTriangleKnobSlider(0, (Rectangle){100, 100, 300, 10}, &sliderValue1, 0.f, 100.f,
                            false, GREEN, TRIANGLE_LEFT, "Slider 1", "Value:");

    // Реєстрація слайдера, напрямок вправо
    RegisterTriangleKnobSlider(1, (Rectangle){500, 100, 10, 300}, &sliderValue2, 0.f, 100.f,
                            true, ORANGE, TRIANGLE_RIGHT, "Slider 2", "Value:");

    BeginDrawing();
    ClearBackground(GRAY);

    // Централізована функція, яка обробляє взаємодію і малює всі слайдери
    UpdateTriangleKnobSlidersAndDraw(font18, 2);

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
