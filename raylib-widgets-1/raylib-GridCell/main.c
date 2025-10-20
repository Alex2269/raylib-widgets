
#include "raylib.h"
#include "GridCell.h"
#include <stdbool.h>
#include <stdlib.h>

int main(void) {
    const int screenWidth = 800;
    const int screenHeight = 600;
    const int cellSize = 40;
    const int padding = 50;

    InitWindow(screenWidth, screenHeight, "GridCell Example with Raylib");

    int gridWidth = screenWidth - 2 * padding;
    int gridHeight = screenHeight - 2 * padding;

    int cellsX = gridWidth / cellSize;
    int cellsY = gridHeight / cellSize;

    GridCell* grid = (GridCell*)malloc(sizeof(GridCell) * cellsX * cellsY);
    if (!grid) {
        CloseWindow();
        return -1;
    }

    int startX = padding + (gridWidth - cellsX * cellSize) / 2;
    int startY = padding + (gridHeight - cellsY * cellSize) / 2;

    initGrid(grid, cellsX, cellsY, cellSize, startX, startY);

    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        // Обробка кліка миші для активації клітинки
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            Vector2 mousePos = GetMousePosition();
            for (int i = 0; i < cellsX * cellsY; i++) {
                GridCell* cell = &grid[i];
                if (mousePos.x >= cell->x && mousePos.x < cell->x + cell->size &&
                    mousePos.y >= cell->y && mousePos.y < cell->y + cell->size) {
                    cell->active = !cell->active;
                break;
                    }
            }
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);

        drawGrid(grid, cellsX, cellsY);

        EndDrawing();
    }

    free(grid);
    CloseWindow();
    return 0;
}
