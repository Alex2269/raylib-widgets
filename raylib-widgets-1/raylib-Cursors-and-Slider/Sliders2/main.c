// main.c
#include "raylib.h"
#include "sliders.h"
#include "osc.h" // Потрібен для констант конфігурації

// Оголошення зовнішніх змінних, які використовуються слайдерами
float horizontal_scale = 1.0f;
int16_t scroll_x_a = 0, scroll_x_b = 0;
int16_t scroll_y_a = 0, scroll_y_b = 0;
int16_t trigger_a = 0;

// Оголошення зовнішньої змінної для стану кнопок (потрібно для кольору тригера)
// extern bool buttonStates[10];

int main(void)
{
    // Ініціалізація вікна
    const int screenWidth = 800;
    const int screenHeight = 600;
    InitWindow(screenWidth, screenHeight, "Raylib Button Example");

    // Створення слайдерів
    Slider slider_y_a = create_slider((Rectangle){WIDTH_RAY + 4, MIN_Y_BORDER, SLIDER_WIDTH / 2, Y_WORKSPACE}, &scroll_y_a, true, MIN_Y_BUFFER, MAX_Y_BUFFER);
    Slider slider_y_b = create_slider((Rectangle){WIDTH_RAY + 20, MIN_Y_BORDER, SLIDER_WIDTH / 2, Y_WORKSPACE}, &scroll_y_b, true, MIN_Y_BUFFER, MAX_Y_BUFFER);
    Slider slider_x_a = create_slider((Rectangle){BEGIN_RAY, 12, X_WORKSPACE, SLIDER_WIDTH}, &scroll_x_a, false, MIN_X_BUFFER, MAX_X_BUFFER);
    Slider slider_x_b = create_slider((Rectangle){BEGIN_RAY, HEIGHT - 28, X_WORKSPACE, SLIDER_WIDTH}, &scroll_x_b, false, MIN_X_BUFFER, MAX_X_BUFFER);
    Slider slider_trigger_a = create_slider((Rectangle){12, MIN_Y_BORDER, SLIDER_WIDTH / 2, Y_WORKSPACE}, &trigger_a, true, TRIGGER_MIN, TRIGGER_MAX);
    Slider slider_scale = create_slider((Rectangle){BEGIN_RAY, 50, X_WORKSPACE, SLIDER_WIDTH}, &horizontal_scale, false, 0.1f, 10.1f);

    // Головний цикл програми
    while (!WindowShouldClose())
    {
        // Оновлення стану слайдерів
        update_slider(&slider_y_a);
        update_slider(&slider_y_b);
        update_slider(&slider_x_a);
        update_slider(&slider_x_b);
        update_slider(&slider_trigger_a);
        update_slider(&slider_scale);

        // Початок малювання
        BeginDrawing();

        ClearBackground(BLACK);

        // Малювання слайдерів
        draw_slider(slider_y_a, YELLOW);
        draw_slider(slider_y_b, GREEN);
        draw_slider(slider_x_a, YELLOW);
        draw_slider(slider_x_b, GREEN);
        // draw_slider(slider_trigger_a, buttonStates[8] ? GREEN : GRAY);
        draw_slider(slider_trigger_a, GRAY);
        draw_slider(slider_scale, BLUE);

        EndDrawing();
    }

    // Закриття вікна
    CloseWindow();

    return 0;
}
