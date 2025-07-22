// sliders.c
#include "sliders.h"

// Змінна для збереження активного слайдера
static Slider *active_slider = NULL;

// Функція для створення нового слайдера
Slider create_slider(Rectangle rect, void *scroll_ptr, bool is_vertical, float min_val, float max_val) {
    Slider new_slider;
    new_slider.rect = rect;
    new_slider.value = 0.5f; // Початкове значення
    new_slider.scroll = scroll_ptr;
    new_slider.is_vertical = is_vertical;
    new_slider.is_active = false;
    new_slider.min_value = min_val;
    new_slider.max_value = max_val;
    return new_slider;
}

// Функція для оновлення стану слайдера (обробка вводу миші)
bool update_slider(Slider *slider) {
    Vector2 mouse_pos = GetMousePosition();
    bool is_mouse_over = CheckCollisionPointRec(mouse_pos, slider->rect);

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && is_mouse_over) {
        slider->is_active = true;
        active_slider = slider;
    }

    if (IsMouseButtonDown(MOUSE_LEFT_BUTTON) && slider->is_active && active_slider == slider) {
        float normalized_value;
        if (slider->is_vertical) {
            normalized_value = (mouse_pos.y - slider->rect.y) / slider->rect.height;
        } else {
            normalized_value = (mouse_pos.x - slider->rect.x) / slider->rect.width;
        }
        slider->value = CLAMP(normalized_value, 0.0f, 1.0f);
        float output_value = slider->min_value + slider->value * (slider->max_value - slider->min_value);
        if (slider->scroll != NULL) {
            if (slider->min_value == 0.1f && slider->max_value == 10.1f) { // Перевірка на слайдер масштабування (приклад)
                *((float*)slider->scroll) = output_value;
            } else {
                *((int16_t*)slider->scroll) = (int16_t)output_value;
            }
        }
        return true;
    }

    if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON) && slider->is_active) {
        slider->is_active = false;
        active_slider = NULL;
    }

    return slider->is_active;
}

// Функція для малювання слайдера
void draw_slider(Slider slider, Color color) {
    uint16_t KNOB = SLIDER_KNOB_SIZE;
    DrawRectangleRoundedLines(slider.rect, 2.0f, 10, SKYBLUE);
    Color knob_color = slider.is_active ? RED : color;
    if (slider.is_vertical) {
        DrawRectangle(slider.rect.x, slider.rect.y + slider.value * slider.rect.height - KNOB / 2, slider.rect.width, KNOB, knob_color);
    } else {
        DrawRectangle(slider.rect.x + slider.value * slider.rect.width - KNOB / 2, slider.rect.y, KNOB, slider.rect.height, knob_color);
    }
}
