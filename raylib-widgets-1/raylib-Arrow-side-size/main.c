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
    InitWindow(300, 300, "Triangle Lines Example");

    Vector2 arrow1 = {100, 50};
    Vector2 arrow2 = {100, 100};
    Vector2 arrow3 = {100, 200};
    Vector2 arrow4 = {100, 250};

    Vector2 arrow = {150, 150};
    Vector2 arrow5 = {250, 150};


    float size = 30.0f;
    float rotation = 0.0f; // градуси

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);

         DrawTriangleLinesRotated(arrow5, size, rotation, GREEN);

         draw_arrow(arrow1, rotation+0, size, YELLOW);
         DrawTriangleArrow(arrow2, rotation+90, size, GREEN);
         draw_arrow(arrow3, rotation+180, size, RED);
         DrawTriangleArrow(arrow4, rotation+270, size, SKYBLUE);

         DrawFilledTriangleRotated(arrow, size, rotation+0, YELLOW);
         DrawFilledTriangleRotated(arrow, size, rotation+90, GREEN);
         DrawFilledTriangleRotated(arrow, size, rotation+180, RED);
         DrawFilledTriangleRotated(arrow, size, rotation+270, SKYBLUE);

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


