#include "raylib.h"

int main() {
    // Ініціалізація вікна
    const int screenWidth = 800;
    const int screenHeight = 450;
    InitWindow(screenWidth, screenHeight, "Raylib UI Example");

    // Змінні для UI елементів
    Rectangle simpleButton = {100, 100, 150, 50};
    Rectangle toggleButton = {300, 100, 150, 50};
    bool toggleState = false;

    Rectangle slider = {100, 200, 200, 20};
    float sliderValue = 0.5f;

    Rectangle vSlider = {350, 100, 20, 200};
    float vSliderValue = 0.5f;

    // Перемикач
    Rectangle rectBtn = (Rectangle){500, 100, 16, 16};
    /* Отримання координат верхнього лівого кута (X,Y),
    /* з прямокутника з додаванням 20 пікселів до Y */
    int rectBtn_x = rectBtn.x;
    int rectBtn_y = rectBtn.y +20;

    bool toggle = false;

    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        // Перевірка натискання простої кнопки
        bool simpleButtonPressed = false;
        if (CheckCollisionPointRec(GetMousePosition(), simpleButton) && IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
            simpleButtonPressed = true;
        }

        // Перевірка натискання кнопки-перемикача
        if (CheckCollisionPointRec(GetMousePosition(), toggleButton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            toggleState = !toggleState;
        }

        // Оновлення повзунка
        if (IsMouseButtonDown(MOUSE_LEFT_BUTTON) &&
            CheckCollisionPointRec(GetMousePosition(), slider)) {
            sliderValue = (GetMouseX() - slider.x) / slider.width;
            if (sliderValue < 0.0f) sliderValue = 0.0f;
            if (sliderValue > 1.0f) sliderValue = 1.0f;
        }

        // Оновлення вертикального повзунка
        if (IsMouseButtonDown(MOUSE_LEFT_BUTTON) &&
            CheckCollisionPointRec(GetMousePosition(), vSlider)) {
            vSliderValue = (GetMouseY() - vSlider.y) / vSlider.height;
            if (vSliderValue < 0.0f) vSliderValue = 0.0f;
            if (vSliderValue > 1.0f) vSliderValue = 1.0f;
        }


        // Перемикач
        if (CheckCollisionPointRec(GetMousePosition(), rectBtn) &&
            IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            toggle = !toggle;
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);

        // Малюємо просту кнопку з зміною кольору при натисканні
        Color simpleButtonColor = simpleButtonPressed ? DARKGRAY : LIGHTGRAY;
        Color simpleTextColor = simpleButtonPressed ? WHITE : BLACK;
        DrawRectangleRounded(simpleButton, 0.5f, 5, simpleButtonColor);
        DrawText("Simple", simpleButton.x + 40, simpleButton.y + 15, 20, simpleTextColor);

        // Малюємо кнопку-перемикач
        Color toggleColor = toggleState ? DARKGRAY : LIGHTGRAY;
        Color toggleTextColor = toggleState ? WHITE : BLACK;
        DrawRectangleRec(toggleButton, toggleColor);
        DrawText("Toggle", toggleButton.x + 40, toggleButton.y + 15, 20, toggleTextColor);

        // Малюємо горизонтальний повзунок
        DrawRectangleRec(slider, GRAY);
        DrawRectangle(slider.x + sliderValue * slider.width - 5, slider.y, 10, slider.height, DARKGRAY);

        // Малюємо вертикальний повзунок
        DrawRectangleRec(vSlider, GRAY);
        DrawRectangle(vSlider.x, vSlider.y + vSliderValue * vSlider.height - 5, vSlider.width, 10, DARKGRAY);

        // Малюємо перемикач
        DrawRectangleRounded(rectBtn, 2.0f, 10, toggle ? GREEN : RED);
        // Малюємо текст перемикача
        DrawText("Toggle", rectBtn_x, rectBtn_y, 20, DARKGRAY);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}

