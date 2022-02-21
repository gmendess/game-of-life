#pragma once

#include <stdlib.h>
#include <stdbool.h>

#define ROWS 20
#define COLS 80
#define MS_PER_FRAME 70 * 1000000

void init_world(bool world[ROWS][COLS]);
bool is_out_of_bound(size_t row, size_t col);
bool should_live(bool cell_value, int neighbors);
int neighbors_number(bool world[ROWS][COLS], ssize_t row, ssize_t col);
void copy_world(bool dest[ROWS][COLS], bool src[ROWS][COLS]);
void draw_world(bool world[ROWS][COLS]);
void clear_terminal();
