// main.c

#include "raylib.h"
#include "slider_widget_circle.h"
#include <stdio.h>
#include <math.h>

#include "all_font.h" // Опис шрифтів як структури RasterFont
#include "glyphs.h"

int LineSpacing = 0; // Міжрядковий інтервал (0 - стандартний)
int spacing = 2; // Відступ між символами, той самий, що передається у DrawPSFText

int main(void)
{
    const int screenWidth = 800;  // Ширина вікна
    const int screenHeight = 450; // Висота вікна

    // Встановлюємо прапорець для мультисемплінгу (покращення якості графіки)
    SetConfigFlags(FLAG_MSAA_4X_HINT);

    // Ініціалізуємо вікно з назвою
    InitWindow(screenWidth, screenHeight, "Raylib slider_widget_circle Example");

    // Встановлюємо цільову частоту кадрів
    SetTargetFPS(60);

    // Масив значень регуляторів (float від 0.1f до 100.0f)
    float values[5] = { 0.0f, 2.25f, 5.5f, 3.0f };

    // Основний цикл програми, виконується поки вікно не закриють
    while (!WindowShouldClose())
    {
        BeginDrawing();          // Починаємо малювання кадру
        ClearBackground(GRAY); // Очищаємо фон білим кольором

        // values[0] = round(values[0] / 0.1) * 0.1;
        // printf("scale: %0.2f\n", values[0]);

        int sliderWidth = 10;
        int sliderHeight = 260;
        Rectangle sliderBounds = { 100, 50, sliderWidth, sliderHeight };
        RegisterCircleKnobSlider(0, sliderBounds, &values[0], 250.0f, -250.0f, true, YELLOW, "tLeft", "tRight");
        RegisterCircleKnobSlider(1, sliderBounds, &values[1], -250.0f, 250.0f, true, GREEN, "tLeft", "tRight");
        RegisterCircleKnobSlider(2, sliderBounds, &values[2], 250.0f, -250.0f, true, RED, "tLeft", "tRight");
        RegisterCircleKnobSlider(3, sliderBounds, &values[3], -250.0f, 250.0f, true, SKYBLUE, "tLeft", "tRight");

        // Централізована функція, яка обробляє взаємодію і малює всі слайдери
        UpdateCircleKnobSlidersAndDraw(TerminusBold18x10_font, 2);

        EndDrawing(); // Завершуємо малювання кадру
    }

    // Закриваємо вікно і звільняємо ресурси
    CloseWindow();

    return 0;
}

