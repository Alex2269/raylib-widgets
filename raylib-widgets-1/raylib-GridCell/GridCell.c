// file GridCell.c

#include "raylib.h"
#include "GridCell.h"
#include <stdbool.h>

// // Структура клітинки сітки
// typedef struct GridCell {
//   int x, y;       // Координати клітинки (положення)
//   int size;       // Розмір клітинки (ширина і висота)
//   Color color;    // Колір клітинки
//   bool active;    // Статус (наприклад, активна чи ні)
// } GridCell;

// Функція ініціалізації клітинок сітки
void initGrid(GridCell* grid, int cellsX, int cellsY, int cellSize, int startX, int startY) {
  for (int row = 0; row < cellsY; row++) {
    for (int col = 0; col < cellsX; col++) {
      int index = row * cellsX + col;
      grid[index].x = startX + col * cellSize;
      grid[index].y = startY + row * cellSize;
      grid[index].size = cellSize;
      grid[index].color = GRAY;
      grid[index].active = false; // за замовчуванням неактивна
    }
  }
}

// Функція малювання сітки клітинок
void drawGrid(GridCell* grid, int cellsX, int cellsY) {
  for (int i = 0; i < cellsX * cellsY; i++) {
    // Малюємо клітинку: якщо активна — інший колір
    Color drawColor = grid[i].active ? SKYBLUE : grid[i].color;

    DrawRectangle(grid[i].x, grid[i].y, grid[i].size, grid[i].size, drawColor);
    DrawRectangleLines(grid[i].x, grid[i].y, grid[i].size, grid[i].size, BLACK);
  }
}
