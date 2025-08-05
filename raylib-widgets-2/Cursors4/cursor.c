// cursor.c
#include "cursor.h"
#include <math.h>
#include <stdio.h>

Cursor InitCursor(float startX, float topY, float width, float height, Color color, int minValue, int maxValue) {
    Cursor cursor;
    cursor.x = startX;
    cursor.topY = topY;
    cursor.width = width;
    cursor.height = height;
    cursor.color = color;
    cursor.isDragging = false;
    cursor.minValue = minValue;
    cursor.maxValue = maxValue;
    cursor.value = minValue + (maxValue - minValue) * ((startX) / GetScreenWidth());
    cursor.y = topY + height + THIN_LINE;
    return cursor;
}

static bool IsMouseOverCursor(Vector2 mousePos, Cursor cursor) {
    return (mousePos.x > cursor.x - cursor.width * 2 && mousePos.x < cursor.x + cursor.width * 2 &&
            mousePos.y > cursor.topY && mousePos.y < cursor.topY + cursor.height);
}

static void UpdateCursorDrag(Cursor *cursor, Vector2 mousePos, Cursor *otherCursor) {
    cursor->x = mousePos.x;

    float minX = cursor->width / 2;
    float maxX = GetScreenWidth() - cursor->width / 2;
    float collisionThreshold = cursor->width * 1.5f;

    if (otherCursor != NULL && cursor->isDragging && cursor != otherCursor) {
        if (cursor->x < otherCursor->x && cursor->x + collisionThreshold > otherCursor->x) {
            cursor->x = otherCursor->x - collisionThreshold;
        } else if (cursor->x > otherCursor->x && cursor->x - collisionThreshold < otherCursor->x) {
            cursor->x = otherCursor->x + collisionThreshold;
        }
    }

    if (cursor->x < minX) cursor->x = minX;
    if (cursor->x > maxX) cursor->x = maxX;

    cursor->value = cursor->minValue + (cursor->maxValue - cursor->minValue) * ((cursor->x - minX) / (maxX - minX));
    if (cursor->value < cursor->minValue) cursor->value = cursor->minValue;
    if (cursor->value > cursor->maxValue) cursor->value = cursor->maxValue;
    cursor->y = cursor->topY + cursor->height + THIN_LINE;
}

void UpdateAndHandleCursors(Cursor *cursors, int count, Vector2 mousePos, bool mouseButtonPressed, bool mouseButtonDown, bool mouseButtonReleased) {
    for (int i = 0; i < count; ++i) {
        Cursor *cursor = &cursors[i];
        Cursor *otherCursor = (count > 1) ? &cursors[(i + 1) % count] : NULL;

        if (mouseButtonPressed && IsMouseOverCursor(mousePos, *cursor)) {
            cursor->isDragging = true;
        }

        if (cursor->isDragging && mouseButtonDown) {
            UpdateCursorDrag(cursor, mousePos, otherCursor);
        }

        if (mouseButtonReleased) {
            cursor->isDragging = false;
        }
    }
}

void DrawCursorsAndDistance(Cursor *cursors, int count, Font font, int fontSize) {

    Vector2 mousePos = GetMousePosition();
    bool mousePressed = IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
    bool mouseDown = IsMouseButtonDown(MOUSE_LEFT_BUTTON);
    bool mouseReleased = IsMouseButtonReleased(MOUSE_LEFT_BUTTON);

    // Оновлення та обробка вводу для всіх курсорів
    UpdateAndHandleCursors(cursors, 2, mousePos, mousePressed, mouseDown, mouseReleased);

    for (int i = 0; i < count; ++i) {
        Cursor cursor = cursors[i];
        DrawRectangle(cursor.x - cursor.width / 2, cursor.topY, cursor.width, cursor.height, cursor.color);
        DrawLine(cursor.x, cursor.topY + cursor.height, cursor.x, cursor.topY + cursor.height + THIN_LINE, BLACK);
    }

    if (count >= 2) {
        Cursor cursorA = cursors[0];
        Cursor cursorB = cursors[1];
        float distance = fabs(cursorA.x - cursorB.x);
        float lineY = cursorA.y;
        Vector2 middlePoint = {(cursorA.x + cursorB.x) / 2, lineY - 15};

        DrawLine(cursorA.x, lineY, cursorB.x, lineY, BLACK);

        float arrowAOffset = (cursorB.x > cursorA.x) ? ARROW_SIZE : -ARROW_SIZE;
        DrawTriangle(
            (Vector2){cursorA.x, lineY},
            (Vector2){cursorA.x + arrowAOffset, lineY + arrowAOffset / 2},
            (Vector2){cursorA.x + arrowAOffset, lineY - arrowAOffset / 2},
            BLACK
        );

        float arrowBOffset = (cursorA.x > cursorB.x) ? ARROW_SIZE : -ARROW_SIZE;
        DrawTriangle(
            (Vector2){cursorB.x, lineY},
            (Vector2){cursorB.x + arrowBOffset, lineY + arrowBOffset / 2},
            (Vector2){cursorB.x + arrowBOffset, lineY - arrowBOffset / 2},
            BLACK
        );

        char distanceText[32];
        sprintf(distanceText, "%i px", (int)distance);
        int textWidth = MeasureText(distanceText, fontSize);
        DrawTextEx(font, distanceText, (Vector2){middlePoint.x - textWidth / 2, middlePoint.y - fontSize - 5}, fontSize, 2, DARKGRAY);
    }
}
