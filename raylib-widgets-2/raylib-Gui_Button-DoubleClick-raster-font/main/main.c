// main.c
#include "raylib.h"
#include "button_extended.h"  // Заголовок з розширеним Gui_ButtonExtended
#include "all_font.h"
#include "glyphs.h"

int spacing = 2;

int main(void)
{
    InitWindow(600, 200, "Button Extended Example");
    SetTargetFPS(60);

    ButtonState btnState = {0};

    bool isLongPress = false;
    bool isDoubleClick = false;
    bool isSingleClick = false;

    double msgDisplayUntil = 0.0;
    const double msgDuration = 1.5; // 1.5 секунди

    const char* msgText = "";

    Rectangle btnRect = { 100, 80, 150, 50 };

    while (!WindowShouldClose())
    {
        double currentTime = GetTime();

        BeginDrawing();
        ClearBackground(RAYWHITE);

        isSingleClick = Gui_ButtonExtended(btnRect, TerminusBold24x12_font, "Press Me", spacing,
                                           SKYBLUE, BLUE, DARKBLUE, (Color){0, 0, 0, 0},
                                           &btnState, &isLongPress, &isDoubleClick);

        // Логіка визначення повідомлення і часу його відображення
        if (isLongPress)
        {
            msgText = "Long Press";
            msgDisplayUntil = currentTime + msgDuration;
        }
        else if (isDoubleClick)
        {
            msgText = "Double Click";
            msgDisplayUntil = currentTime + msgDuration;
        }
        else if (isSingleClick)
        {
            msgText = "Single Click";
            msgDisplayUntil = currentTime + msgDuration;
        }

        // Виводимо повідомлення, якщо таймер не минув
        if (currentTime < msgDisplayUntil)
        {
            DrawTextScaled(TerminusBold24x12_font, 300, 90, msgText, spacing, 1, BLACK);
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
