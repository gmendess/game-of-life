#include <stdio.h>
#include <memory.h>
#include <time.h>
#include <unistd.h>
#include "game-of-life.h"

int main(void) {

    srand(time(NULL));

    struct timespec ts = { .tv_sec = 0, .tv_nsec = MS_PER_FRAME };
    bool world[ROWS][COLS], world_next_generation[ROWS][COLS];

    init_world(world);

    while(true) {
        clear_terminal();
        draw_world(world);

        for(size_t i = 0; i < ROWS; i++) {
            for(size_t j = 0; j < COLS; j++) {
                int neighbors = neighbors_number(world, i, j);
                world_next_generation[i][j] = should_live(world[i][j], neighbors);
            }
        }

        nanosleep(&ts, NULL);
        copy_world(world, world_next_generation);
    }

    return EXIT_SUCCESS;
}

void init_world(bool world[ROWS][COLS]) {
    for(size_t i = 0; i < ROWS; i++)
        for(size_t j = 0; j < COLS; j++)
            world[i][j] = rand() % 2;
}

int neighbors_number(bool world[ROWS][COLS], ssize_t row, ssize_t col) {
    int neighbors = 0;

    for(ssize_t i = row - 1; i <= row + 1; i++)
        for(ssize_t j = col - 1; j <= col + 1; j++) {
            if(is_out_of_bound(i, j)) continue;
            neighbors += world[i][j];
        }

    neighbors -= world[row][col];
    return neighbors;
}

bool should_live(bool cell_value, int neighbors) {
    return (neighbors == 2 && cell_value) || neighbors == 3;
}

bool is_out_of_bound(size_t row, size_t col) {
    return !((row >= 0 && row < ROWS) && (col >= 0 && col < COLS));
}

void copy_world(bool dest[ROWS][COLS], bool src[ROWS][COLS]) {
    memcpy(dest, src, ROWS * COLS * sizeof(src[0][0]));
}

void draw_world(bool world[ROWS][COLS]) {
    for(size_t i = 0; i < COLS + 2; i++)
        putchar('-');
    putchar(10);

    for(size_t i = 0; i < ROWS; i++) {
        putchar('|');
        for(size_t j = 0; j < COLS; j++)
            putchar(world[i][j] ? 'o' : ' ');

        printf("|\n");
    }

    for(size_t i = 0; i < COLS + 2; i++)
        putchar('-');
    putchar(10);
}

void clear_terminal() {
    puts("\033[2J\033[1;1H");
}