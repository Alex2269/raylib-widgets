#include "raylib.h"

// Структура слайдера
typedef struct Slider {
    Rectangle bar;       // Прямокутник, що задає область "шкали" слайдера
    Rectangle knob;      // Прямокутник, що задає область "ручки" (ползунка)
    float minValue;      // Мінімальне значення слайдера
    float maxValue;      // Максимальне значення слайдера
    float value;         // Поточне значення слайдера
    bool vertical;       // Вертикальний чи горизонтальний слайдер
    bool dragging;       // Чи відбувається зараз перетягування
} Slider;

// Ініціалізація слайдера
void SliderInit(Slider *slider, float x, float y, float length, float thickness, float minValue, float maxValue, bool vertical) {
    slider->vertical = vertical;
    slider->minValue = minValue;
    slider->maxValue = maxValue;
    slider->value = minValue;  // Початкове значення — мінімум
    slider->dragging = false;  // Перетягування не активне

    if (vertical) {
        // Для вертикального слайдера бар — вертикальна смуга
        slider->bar = (Rectangle){ x, y, thickness, length };
        // Ручка ширша за бар, щоб було зручно тягнути
        slider->knob = (Rectangle){ x - thickness / 2, y, thickness * 2, thickness };
    } else {
        // Для горизонтального слайдера бар — горизонтальна смуга
        slider->bar = (Rectangle){ x, y, length, thickness };
        // Ручка вища за бар
        slider->knob = (Rectangle){ x, y - thickness / 2, thickness, thickness * 2 };
    }
}

// Перевірка, чи знаходиться курсор миші над баром слайдера
bool IsMouseOverSliderBar(Slider *slider, Vector2 mousePos) {
    return CheckCollisionPointRec(mousePos, slider->bar);
}

// Оновлення стану слайдера (обробка миші)
void SliderUpdate(Slider *slider) {
    Vector2 mousePos = GetMousePosition();

    // Якщо натиснули ліву кнопку миші
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        // Перевіряємо, чи курсор знаходиться над баром слайдера
        if (IsMouseOverSliderBar(slider, mousePos)) {
            slider->dragging = true;  // Починаємо перетягування
        }
    }
    // Якщо відпустили кнопку — припиняємо перетягування
    if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
        slider->dragging = false;
    }

    // Якщо зараз перетягуємо слайдер
    if (slider->dragging) {
        if (slider->vertical) {
            // Обмежуємо позицію миші в межах бару по вертикалі
            float pos = mousePos.y;
            if (pos < slider->bar.y) pos = slider->bar.y;
            if (pos > slider->bar.y + slider->bar.height) pos = slider->bar.y + slider->bar.height;
            // Оновлюємо позицію ручки по вертикалі
            slider->knob.y = pos - slider->knob.height / 2;

            // Обчислюємо нормалізоване значення від 0 до 1
            float t = (slider->knob.y + slider->knob.height / 2 - slider->bar.y) / slider->bar.height;
            // Визначаємо поточне значення слайдера (з урахуванням напрямку)
            slider->value = slider->maxValue - t * (slider->maxValue - slider->minValue);
        } else {
            // Обмежуємо позицію миші в межах бару по горизонталі
            float pos = mousePos.x;
            if (pos < slider->bar.x) pos = slider->bar.x;
            if (pos > slider->bar.x + slider->bar.width) pos = slider->bar.x + slider->bar.width;
            // Оновлюємо позицію ручки по горизонталі
            slider->knob.x = pos - slider->knob.width / 2;

            // Обчислюємо нормалізоване значення від 0 до 1
            float t = (slider->knob.x + slider->knob.width / 2 - slider->bar.x) / slider->bar.width;
            // Визначаємо поточне значення слайдера
            slider->value = slider->minValue + t * (slider->maxValue - slider->minValue);
        }
    }
}

// Малювання слайдера на екрані
void SliderDraw(Slider *slider) {
    DrawRectangleRec(slider->bar, LIGHTGRAY);  // Малюємо бар
    DrawRectangleRec(slider->knob, DARKGRAY);  // Малюємо ручку
}

int main(void) {
    InitWindow(800, 450, "raylib slider capture entire bar example");

    Slider hSlider, vSlider;
    // Ініціалізуємо горизонтальний слайдер
    SliderInit(&hSlider, 100, 200, 300, 10, 0, 100, false);
    // Ініціалізуємо вертикальний слайдер
    SliderInit(&vSlider, 500, 100, 200, 10, 0, 100, true);

    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        // Оновлюємо слайдери (обробляємо введення)
        SliderUpdate(&hSlider);
        SliderUpdate(&vSlider);

        BeginDrawing();
        ClearBackground(RAYWHITE);

        // Виводимо поточні значення слайдерів
        DrawText(TextFormat("Horizontal Slider Value: %.2f", hSlider.value), 100, 170, 20, BLACK);
        DrawText(TextFormat("Vertical Slider Value: %.2f", vSlider.value), 500, 70, 20, BLACK);

        // Малюємо слайдери
        SliderDraw(&hSlider);
        SliderDraw(&vSlider);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}

