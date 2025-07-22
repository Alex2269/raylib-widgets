#include "raylib.h"
#include "psf_font.h"
#include "sliders_ex.h"

PSF_Font font18;
int spacing = 2;

void UpdateDrawFrame(void) {
    Vector2 mousePos = GetMousePosition();
    bool mousePressed = IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
    bool mouseDown = IsMouseButtonDown(MOUSE_LEFT_BUTTON);
    bool mouseReleased = IsMouseButtonReleased(MOUSE_LEFT_BUTTON);

    UpdateSliders(mousePos, mousePressed, mouseDown, mouseReleased);

    BeginDrawing();
    ClearBackground(GRAY);

    // Малюємо слайдери
    DrawSliders(font18);

    EndDrawing();
}

int main(void) {
    const int screenWidth = 800;
    const int screenHeight = 600;

    InitWindow(screenWidth, screenHeight, "Raylib Slider LIFO Example");

    font18 = LoadPSFFont("fonts/Uni3-TerminusBold18x10.psf");

    InitSliders();

    while (!WindowShouldClose()) {
        UpdateDrawFrame();
    }

    UnloadPSFFont(font18);
    CloseWindow();

    return 0;
}

