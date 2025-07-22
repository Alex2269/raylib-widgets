#include "raylib.h"
#include "slider.h" // Тепер містить оголошення для слайдера
#include <stdio.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 450
#define FONT_SIZE 24

Font font;

Font LoadFontUnicode(char * fnt, int fontSize);

Font LoadFontUnicode(char * fnt, int fontSize)
{
    int codepointCount = 512;
    int codepoints[512] = { 0 };

    for (int i = 0; i < 95; i++) codepoints[i] = 32 + i;
    for (int i = 96; i < 351; i++) codepoints[i] = 928 + i;
    Font font = LoadFontEx(fnt, fontSize, codepoints, codepointCount);
    SetTextureFilter(font.texture, TEXTURE_FILTER_BILINEAR);
    SetTextLineSpacing(0);
    return font;
}

int main(void)
{
    // Ініціалізація вікна
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Raylib Slider Widget");

    font = LoadFontUnicode("bold.ttf", FONT_SIZE);

    SetTargetFPS(60);

    // Визначаємо позицію та розмір слайдера з main
    Rectangle sliderBounds = { 100, 100, 600, 40 };
    int sliderMinValue = 0;
    int sliderMaxValue = 100;

    // Ініціалізація віджета слайдера
    SliderWidget slider = InitSliderWidget(sliderBounds, sliderMinValue, sliderMaxValue);

    // Отримуємо поточний стан кнопок миші
    bool mouseButtonPressed = false;
    bool mouseButtonDown = false;
    bool mouseButtonReleased = false;
    Vector2 mousePosition = { 0.0f, 0.0f };

    // Головний цикл гри
    while (!WindowShouldClose())
    {
        // Оновлення
        Vector2 currentMousePosition = GetMousePosition();
        mouseButtonPressed = IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
        mouseButtonDown = IsMouseButtonDown(MOUSE_LEFT_BUTTON);
        mouseButtonReleased = IsMouseButtonReleased(MOUSE_LEFT_BUTTON);

        // Обробка вводу миші для віджета слайдера
        HandleSliderWidgetMouseInput(&slider, currentMousePosition, mouseButtonPressed, mouseButtonDown, mouseButtonReleased);

        // Малювання
        BeginDrawing();
            ClearBackground(RAYWHITE);

            // Малюємо віджет слайдера
            DrawSliderWidget(slider);

            // Виводимо значення слайдера
            DrawTextEx(font, TextFormat("Значення зліва: %i", slider.leftValue), (Vector2){100, 150}, FONT_SIZE, 2, BLACK);
            DrawTextEx(font, TextFormat("Значення справа: %i", slider.rightValue), (Vector2){100, 180}, FONT_SIZE, 2, BLACK);
            DrawTextEx(font, "Перетягуйте ручки слайдера", (Vector2){100, 50}, FONT_SIZE, 2, GRAY);

        EndDrawing();
    }

    // De-Initialization
    UnloadFont(font);
    CloseWindow();

    return 0;
}

