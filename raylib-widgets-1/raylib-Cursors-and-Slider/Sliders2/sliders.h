// sliders.h
#ifndef SLIDERS_H
#define SLIDERS_H

#include "raylib.h"
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#define SLIDER_WIDTH 24 // Ширина повзунка
#define SLIDER_KNOB_SIZE 10

#define CLAMP(value, min, max) ((value) < (min) ? (min) : ((value) > (max) ? (max) : (value)))

// Структура Slider
typedef struct {
    Rectangle rect;         // Прямокутник, що визначає область повзунка
    float value;            // Поточне значення повзунка (від 0.0 до 1.0)
    void *scroll;           // Вказівник на змінну, що змінюється при русі повзунка
    bool is_vertical;       // Ознака вертикального (true) або горизонтального (false) повзунка
    bool is_active;         // Чи активний слайдер для взаємодії
    float min_value;        // Мінімальне значення, яке може приймати слайдер (у вихідних одиницях)
    float max_value;        // Максимальне значення, яке може приймати слайдер (у вихідних одиницях)
} Slider;

// Функція для створення нового слайдера
Slider create_slider(Rectangle rect, void *scroll_ptr, bool is_vertical, float min_val, float max_val);
// Функція для оновлення стану слайдера (обробка вводу миші)
bool update_slider(Slider *slider);
// Функція для малювання слайдера
void draw_slider(Slider slider, Color color);

#endif // SLIDERS_H
