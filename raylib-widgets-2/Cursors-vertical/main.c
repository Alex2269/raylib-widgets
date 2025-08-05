// main.c

#include "raylib.h"
#include "cursor.h"
#include <stdio.h>

// Функція для додавання нової ручки з кольором по черзі
void AddNewCursor(CursorArray *arr, float fixedX, float startY) {
    Color colors[] = { RED, BLUE, GREEN, ORANGE, PURPLE, MAROON };
    int colorIndex = arr->count % (sizeof(colors) / sizeof(colors[0]));
    Cursor newCursor = InitCursor(fixedX, startY, 20, 10, colors[colorIndex], 0, 100);
    AddCursor(arr, newCursor);
}

int main(void) {
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "Vertical Slider with Sticky Cursors");
    SetTargetFPS(60);

    Font font = GetFontDefault();
    int fontSize = 20;

    CursorArray cursors;
    InitCursorArray(&cursors, 4);

    float fixedX = screenWidth - 50; // Ручки справа, 50 пікселів від краю

    // Додаємо дві початкові ручки
    AddNewCursor(&cursors, fixedX, screenHeight / 3.0f);
    AddNewCursor(&cursors, fixedX, screenHeight / 1.5f);

    while (!WindowShouldClose()) {
        Vector2 mousePos = GetMousePosition();
        bool mousePressed = IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
        bool mouseDown = IsMouseButtonDown(MOUSE_LEFT_BUTTON);
        bool mouseReleased = IsMouseButtonReleased(MOUSE_LEFT_BUTTON);

        // Оновлюємо курсори (перетягування, прилипання, колізії)
        UpdateAndHandleCursors(&cursors, mousePos, mousePressed, mouseDown, mouseReleased);

        BeginDrawing();
        ClearBackground(RAYWHITE);

        // Малюємо курсори та лінії
        DrawCursorsAndLines(&cursors, font, fontSize);

        // Інструкції для користувача
        DrawText("ЛКМ перетягує ручки", 10, 10, 20, DARKGRAY);
        DrawText("Натисніть '+' щоб додати ручку", 10, 40, 20, DARKGRAY);
        DrawText("Натисніть '-' щоб видалити останню ручку", 10, 70, 20, DARKGRAY);

        EndDrawing();

        // Додавання/видалення ручок клавішами
        if (IsKeyPressed(KEY_KP_ADD) || IsKeyPressed(KEY_EQUAL)) {
            AddNewCursor(&cursors, fixedX, screenHeight / 2.0f);
        }
        if ((IsKeyPressed(KEY_KP_SUBTRACT) || IsKeyPressed(KEY_MINUS)) && cursors.count > 0) {
            RemoveCursor(&cursors, cursors.count - 1);
        }
    }

    FreeCursorArray(&cursors);
    CloseWindow();

    return 0;
}

