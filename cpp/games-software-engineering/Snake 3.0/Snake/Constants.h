#pragma once
const int WINDOW_WIDTH = 1000;
const int WINDOW_HEIGHT = 800;
const int CELL_SIZE = 32;           // Each grid square is 32x32 pixels
const int GRID_COLS = WINDOW_WIDTH / CELL_SIZE;   // 20 columns
const int GRID_ROWS = WINDOW_HEIGHT / CELL_SIZE;   // 20 rows
const int INITIAL_SNAKE_LENGTH = 12;
const float MOVE_INTERVAL = 0.15f;     // Seconds between each snake step+

enum Direction { UP, DOWN, LEFT, RIGHT };