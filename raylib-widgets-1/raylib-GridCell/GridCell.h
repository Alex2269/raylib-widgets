// file GridCell.h

#ifndef GRIDCELL_H_
#define GRIDCELL_H_

// Структура клітинки сітки
typedef struct GridCell {
    int x, y;       // Координати клітинки (положення)
    int size;       // Розмір клітинки (ширина і висота)
    Color color;    // Колір клітинки
    bool active;    // Статус (наприклад, активна чи ні)
} GridCell;

// Функція ініціалізації клітинок сітки
void initGrid(GridCell* grid, int cellsX, int cellsY, int cellSize, int startX, int startY);
// Функція малювання сітки клітинок
void drawGrid(GridCell* grid, int cellsX, int cellsY);

#endif // GRIDCELL_H_
