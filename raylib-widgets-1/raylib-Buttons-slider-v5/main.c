#include "stdint.h"
#include "raylib.h"

#define WIDTH 800
#define HEIGHT 480

#define CELL_SIZE 40
#define BEGIN_RAY (CELL_SIZE * 4)
#define WIDTH_RAY (WIDTH - (CELL_SIZE-1))
#define HALF_RAY ((WIDTH_RAY - BEGIN_RAY) / 2)
#define MIN_Y_BORDER CELL_SIZE
#define MAX_Y_BORDER (HEIGHT - CELL_SIZE)

// Константи для кращої читабельності
#define ADC_RESOLUTION 4096 // Роздільна здатність АЦП (Analog-to-Digital Converter)
#define BUFFER_NOISE_REDUCTION 1.0f // Параметр для зменшення шумів в буфері
#define TEST_SIGNAL_MULTIPLIER 0.0f // Множник для тестового сигналу
#define TEST_SIGNAL_OFFSET 2.0f // Зсув для тестового сигналу
#define TRIGGER_MIN 50 // Мінімальне значення для тригера
#define TRIGGER_MAX 4050 // Максимальне значення для тригера

// Інші константи для меж та розмірів
#define MAX_X_BUFFER (WIDTH_RAY) // Максимальна межа для буфера по осі X
#define MIN_X_BUFFER (0) // Мінімальна межа для буфера по осі X
#define MAX_Y_BUFFER (HEIGHT - CELL_SIZE) // Максимальна межа для буфера по осі Y
#define MIN_Y_BUFFER (CELL_SIZE) // Мінімальна межа для буфера по осі Y
// #define Y_WORKSPACE (MAX_Y_BORDER - MIN_Y_BORDER)
#define Y_WORKSPACE (MAX_Y_BUFFER - MIN_Y_BUFFER) // Робоча область по осі Y
#define X_WORKSPACE (WIDTH - (BEGIN_RAY + CELL_SIZE)) // Робоча область по осі X

// Макрос для обмеження значення в межах
#define CLAMP(value, min, max) ((value) < (min) ? (min) : ((value) > (max) ? (max) : (value)))

uint16_t buffering = 0; // Статус буферизації (1 - буферизація включена, 0 - вимкнена)

int16_t scroll_x_a = 0, scroll_x_b = 0; // Зміщення буферів по осі X для каналів A і B
int16_t scroll_y_a = 0, scroll_y_b = 0; // Зміщення буферів по осі Y для каналів A і B

// Вертикальні повзунки для налаштування каналів
Rectangle vSlider_a = {BEGIN_RAY - 24, MIN_Y_BORDER, 16, MAX_Y_BORDER - CELL_SIZE}; // Повзунок для каналу A
float vSliderValue_a = 0.5f; // Початкове значення повзунка для каналу A (центр)
Rectangle vSlider_b = {WIDTH_RAY + 12, MIN_Y_BORDER, 16, MAX_Y_BORDER - CELL_SIZE}; // Повзунок для каналу B
float vSliderValue_b = 0.5f; // Початкове значення повзунка для каналу B (центр)

// Горизонтальні повзунки для налаштування каналів
Rectangle hSlider_a = {BEGIN_RAY, 12, WIDTH - BEGIN_RAY - CELL_SIZE, 16}; // Горизонтальний повзунок для каналу A
float hSliderValue_a = 0.5f; // Початкове значення горизонтального повзунка для каналу A (центр)
Rectangle hSlider_b = {BEGIN_RAY, HEIGHT - 28, WIDTH - BEGIN_RAY - CELL_SIZE, 16}; // Горизонтальний повзунок для каналу B
float hSliderValue_b = 0.5f; // Початкове значення горизонтального повзунка для каналу B (центр)

// Повзунок для налаштування тригера
Rectangle vSlider_trig_a = {24, MIN_Y_BORDER, 16, MAX_Y_BORDER - CELL_SIZE};
float vSlider_trigValue_a = 0.5f; // Початкове значення повзунка тригера (центр)
uint16_t trigger_a = 0; // Змінна тригера

Rectangle triggerBtn; // Кнопка для тригера
bool toggle = false; // Стан кнопки тригера (увімкнено/вимкнено)




/* Оновлення вертикальних повзунків для каналів A і B */
void update_vertical_sliders(void) {
    if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
        if (CheckCollisionPointRec(GetMousePosition(), vSlider_a)) {
            vSliderValue_a = (GetMouseY() - vSlider_a.y) / vSlider_a.height;
            vSliderValue_a = fmaxf(0.0f, fminf(1.0f, vSliderValue_a));
            scroll_y_a = (int16_t)(vSliderValue_a * Y_WORKSPACE - Y_WORKSPACE / 2);
        }
        if (CheckCollisionPointRec(GetMousePosition(), vSlider_b)) {
            vSliderValue_b = (GetMouseY() - vSlider_b.y) / vSlider_b.height;
            vSliderValue_b = fmaxf(0.0f, fminf(1.0f, vSliderValue_b));
            scroll_y_b = (int16_t)(vSliderValue_b * Y_WORKSPACE - Y_WORKSPACE / 2);
        }
    }
}

/* Оновлення горизонтальних повзунків для каналів A і B */
void update_horizontal_sliders(void) {
    if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
        if (CheckCollisionPointRec(GetMousePosition(), hSlider_a)) {
            hSliderValue_a = (GetMouseX() - hSlider_a.x) / hSlider_a.width;
            hSliderValue_a = fmaxf(0.0f, fminf(1.0f, hSliderValue_a));
            scroll_x_a = (int16_t)(hSliderValue_a * WIDTH_RAY);
            scroll_x_a = CLAMP(scroll_x_a, MIN_X_BUFFER, MAX_X_BUFFER); // Обмежуємо scroll_x_a
        }
        if (CheckCollisionPointRec(GetMousePosition(), hSlider_b)) {
            hSliderValue_b = (GetMouseX() - hSlider_b.x) / hSlider_b.width;
            hSliderValue_b = fmaxf(0.0f, fminf(1.0f, hSliderValue_b));
            scroll_x_b = (int16_t)(hSliderValue_b * WIDTH_RAY);
            scroll_x_b = CLAMP(scroll_x_b, MIN_X_BUFFER, MAX_X_BUFFER); // Обмежуємо scroll_x_b
        }
    }
}

/* Оновлення вертикального повзунка для тригера */
void update_trigger_slider(void) {
    // Оновлення вертикального повзунка тригера
    if (IsMouseButtonDown(MOUSE_LEFT_BUTTON) &&
        CheckCollisionPointRec(GetMousePosition(), vSlider_trig_a)) {
        vSlider_trigValue_a = (GetMouseY() - vSlider_trig_a.y) / vSlider_trig_a.height;
        if (vSlider_trigValue_a < 0.0f) vSlider_trigValue_a = 0.0f;
        if (vSlider_trigValue_a > 1.0f) vSlider_trigValue_a = 1.0f;
        trigger_a = ADC_RESOLUTION - (int16_t)(vSlider_trigValue_a * ADC_RESOLUTION);
        printf("trigger_a: %d\n", trigger_a);
    }
}

//================ Графічні функції =========================

/* Малюємо вертикальні повзунки */
void draw_sliders_vertical(void) {
    // Малюємо вертикальний повзунок A
    DrawRectangleRec(vSlider_a, DARKGRAY);
    DrawRectangle(vSlider_a.x, vSlider_a.y + vSliderValue_a * vSlider_a.height - 5, vSlider_a.width, 4, YELLOW);

    // Малюємо вертикальний повзунок B
    DrawRectangleRec(vSlider_b, DARKGRAY);
    DrawRectangle(vSlider_b.x, vSlider_b.y + vSliderValue_b * vSlider_b.height - 5, vSlider_b.width, 4, GREEN);
}

/* Малюємо горизонтальні повзунки */
void draw_sliders_horizontal(void) {
    //* Малюємо горизонтальні повзунки */ A
    DrawRectangleRec(hSlider_a, DARKGRAY);
    DrawRectangle(hSlider_a.x + hSliderValue_a * hSlider_a.width - 5, hSlider_a.y, 4, hSlider_a.height, YELLOW);

    //* Малюємо горизонтальні повзунки */ B
    DrawRectangleRec(hSlider_b, DARKGRAY);
    DrawRectangle(hSlider_b.x + hSliderValue_b * hSlider_b.width - 5, hSlider_b.y, 4, hSlider_b.height, GREEN);
}

/* Малюємо вертикальний повзунок тригера */
void draw_slider_trigger(void) {
    // Малюємо вертикальний повзунок тригера
    DrawRectangleRec(vSlider_trig_a, DARKGRAY);
    DrawRectangle(vSlider_trig_a.x, vSlider_trig_a.y + vSlider_trigValue_a * vSlider_trig_a.height - 5, vSlider_trig_a.width, 4, GREEN);
}

/* Малювання перемикача трігера */
void draw_trigger_button(bool toggle) {
    // Перемикач трігера
    // Розміщуємо кнопку тригера
    triggerBtn = (Rectangle){24, 10, 16, 16};
    static int triggerBtn_x;
    static int triggerBtn_y;
    triggerBtn_x = triggerBtn.x + 20; // Координати кнопки по осі X
    triggerBtn_y = triggerBtn.y; // Координати кнопки по осі Y

    // Малюємо перемикач
    DrawRectangleRounded(triggerBtn, 2.0f, 10, toggle ? GREEN : RED);
    // Малюємо текст перемикача
    DrawText("Buffering", triggerBtn_x, triggerBtn_y, 20, toggle ? GREEN : DARKGRAY);
}

/* Функція для основного циклу осцилографа */
void oscil_run(void) {
    draw_trigger_button(toggle); // Малювання кнопки тригера

    // Обробка натискання на кнопку тригера
    if (CheckCollisionPointRec(GetMousePosition(), triggerBtn) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
    toggle = !toggle; // Перемикання стану кнопки
    printf("Тригер перемикається: %d\n", toggle);
    }

    buffering = toggle ? 1 : 0;  // Встановлення статусу буферизації

    // Обробка вертикальних повзунків для каналів A та B
    update_vertical_sliders();
    // Обробка горизонтальних повзунків для каналів A та B
    update_horizontal_sliders();
    // Обробка вертикального повзунка тригера
    update_trigger_slider();

    // // Якщо увімкнена буферизація, викликаємо відповідну функцію
    // if (buffering) {
    //     // Оновлюємо графік
    //     run_buffering();
    // } else {
    //     // Оновлюємо без буферизації
    //     run_no_buffering();
    // }
}

int main() {
    // Ініціалізація вікна
    InitWindow(WIDTH, HEIGHT, "Sliders");
    SetTargetFPS(60);

    // Початкове очищення екрану та малювання сітки
    DrawRectangle(0, 0, WIDTH, HEIGHT, BLACK); // Очищаємо екран чорним кольором

    // Основний цикл осцилографа
    while (!WindowShouldClose()) {
        draw_sliders_vertical(); // Малювання вертикальних повзунків
        draw_sliders_horizontal(); // Малювання горизонтальних повзунків
        draw_slider_trigger(); // Малювання повзунка тригера

        BeginDrawing(); // Початок малювання кадру
        ClearBackground(BLACK); // Очищення фону
        oscil_run(); // Запуск основної функції осцилографа
        EndDrawing(); // Кінець малювання кадру
    }
    CloseWindow(); // Закриття вікна

    return 0;
}

