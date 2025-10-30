// main.c

#include "raylib.h"
#include "knob_gui_int.h"
#include "LoadFontUnicode.h"
#include <stdio.h>

int fontSize = 24;   // Розмір шрифту для тексту
int LineSpacing = 0; // Міжрядковий інтервал (0 - стандартний)
Font font;           // Об'єкт шрифту

int main(void)
{
    const int screenWidth = 800;  // Ширина вікна
    const int screenHeight = 450; // Висота вікна

    // Встановлюємо прапорець для мультисемплінгу (покращення якості графіки)
    SetConfigFlags(FLAG_MSAA_4X_HINT);

    // Ініціалізуємо вікно з назвою
    InitWindow(screenWidth, screenHeight, "Raylib Knob Example");

    // Завантажуємо шрифт з підтримкою Unicode
    font = LoadFontUnicode("bold.ttf", fontSize, LineSpacing);

    // Встановлюємо цільову частоту кадрів
    SetTargetFPS(60);

    // Масив значень регуляторів (float від 0.0 до 100.0)
    int values[4] = { 0, 25, 50, 75 };

    // Підписи для кожного каналу
    const char *labels[4] = { "Канал 1", "Канал 2", "Канал 3", "Канал 4" };

    // Позиції центрів регуляторів на екрані
    Vector2 positions[4] = {
        {150, 200},
        {300, 200},
        {450, 200},
        {600, 200}
    };

    float radius = 30.0f; // Радіус регуляторів

    // Основний цикл програми, виконується поки вікно не закриють
    while (!WindowShouldClose())
    {
        BeginDrawing();          // Починаємо малювання кадру
        ClearBackground(RAYWHITE); // Очищаємо фон білим кольором

        // Малюємо заголовок у верхньому лівому куті
        DrawText("Knob Control Example", 10, 10, 20, DARKGRAY);

        // Відображаємо і обробляємо кожен регулятор
        for (int i = 0; i < 4; i++)
        {
            // Викликаємо функцію для відображення регулятора і обробки подій
            Gui_Knob_Channel_Int(i, (int)positions[i].x, (int)positions[i].y, labels[i], radius, &values[i], 0, 100, true, BLACK);

            // Виводимо текст із поточним значенням регулятора поруч
            DrawTextEx(font, TextFormat("%s value changed: %d", labels[i], values[i]),
                       (Vector2){10, 40 + 30 * i}, fontSize, 2, RED);
        }

        EndDrawing(); // Завершуємо малювання кадру
    }

    // Закриваємо вікно і звільняємо ресурси
    CloseWindow();

    return 0;
}

