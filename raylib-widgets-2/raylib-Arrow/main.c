#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

#include "raylib.h"

#include "main.h"

#include "DrawTriangleArrow.h"
#include "DrawIsoscelesTriangle.h"

int main(void) {
    InitWindow(800, 600, "Triangle Lines Example");

    Vector2 center = {200, 100};
    float size = 30.0f;
    float rotation = 270.0f; // градуси

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);

        // DrawTriangleLinesRotated(center, size, rotation, GREEN);
        // DrawTriangleArrow(center, rotation, size, GREEN);
         DrawTriangleArrow(center, rotation, size, GREEN);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}

/*
int main(void) {
    InitWindow(800, 600, "Filled Rotated Triangle");

    Vector2 center = {400, 300};
    float size = 100.0f;
    float rotation = 30.0f;

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        DrawFilledTriangleRotated(center, size, rotation, GREEN);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}*/


