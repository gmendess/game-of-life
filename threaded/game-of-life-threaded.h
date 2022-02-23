#pragma once

#include <stdlib.h>
#include <stdbool.h>

#define THREADS 5
#define ROWS 20
#define COLS 80
#define MS_PER_FRAME 75 * 1000000

void init_world();
bool is_out_of_bound(size_t row, size_t col);
bool should_live(bool cell_value, int neighbors);
int neighbors_number(ssize_t row, ssize_t col);
void copy_world();
void draw_world();
void clear_terminal();
void* update_section(void* args);
