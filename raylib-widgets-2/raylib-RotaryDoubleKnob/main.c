// main.c

#include <stdint.h>
#include "raylib.h"
#include "dv_knob.h"
#include "LoadFontUnicode.h"

#define SCREEN_WIDTH  400
#define SCREEN_HEIGHT 300
#define FONT_SIZE     24
#define LINE_SPACING  0

Font font;

/**
 * Головна функція програми.
 * Ініціалізує вікно, шрифт та подвійний регулятор,
 * запускає цикл обробки подій та рендерингу.
 */
int main() {
    // Ініціалізація графічного вікна
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Dual Vernier Knob Example");

    // Завантаження Unicode-шрифту із заданим розміром та міжрядковим інтервалом
    font = LoadFontUnicode("bold.ttf", FONT_SIZE, LINE_SPACING);

    // Встановлення цільової частоти кадрів у секунду (60 FPS)
    SetTargetFPS(60);

    // Ініціалізація верньєрного регулятора в центрі вікна з радіусом 60 пікселів
    DualVernierKnob vernierKnob = InitDualVernierKnob(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 60);

    // Основний цикл поки не вийде команда закрити вікно
    while (!WindowShouldClose()) {
        // Оновлення стану регулятора (обробка введення)
        vernierKnob.value = UpdateDualVernierKnob(&vernierKnob, true);

        BeginDrawing();
        ClearBackground(RAYWHITE);

        // Відображення регулятора на екрані
        DrawDualVernierKnob(vernierKnob);

        // Підказка користувачу про взаємодію з регулятором
        DrawTextEx(font, "Клікніть та обертайте\nвнутрішню/зовнішню ручку",
                   (Vector2){20, 20}, FONT_SIZE, 2, DARKGRAY);

        // Відображення поточного числового значення регулятора
        DrawTextEx(font, TextFormat("Значення: %lld", (long long)vernierKnob.value),
                   (Vector2){20, 240}, FONT_SIZE, 2, DARKGRAY);

        EndDrawing();
    }

    // Закриття вікна і звільнення ресурсів
    CloseWindow();
    return 0;
}

