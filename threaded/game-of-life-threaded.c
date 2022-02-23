#include <stdio.h>
#include <memory.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>
#include <assert.h>
#include "game-of-life-threaded.h"

bool world[ROWS][COLS], world_next_generation[ROWS][COLS];
struct timespec ts = { .tv_sec = 0, .tv_nsec = MS_PER_FRAME };
pthread_barrier_t barrier = { 0 };

int main(void) {
    assert(ROWS % THREADS == 0);

    pthread_t threads[THREADS] = { 0 };

    init_world(world);
    pthread_barrier_init(&barrier, NULL, THREADS);

    for(size_t i = 0; i < THREADS; i++) {
        size_t offset = i * (ROWS / THREADS);
        pthread_create(&threads[i], NULL, update_section, (void*) offset);
    }

    pthread_join(threads[0], NULL);
    return EXIT_SUCCESS;
}

void* update_section(void* args) {
    size_t offset = (size_t) args;

    while(true) {
        if(pthread_barrier_wait(&barrier) == PTHREAD_BARRIER_SERIAL_THREAD) {
            copy_world();
            draw_world();
            nanosleep(&ts, NULL);
        }

        pthread_barrier_wait(&barrier);

        for(size_t i = offset; i < offset + (ROWS / THREADS); i++) {
            for(size_t j = 0; j < COLS; j++) {
                int neighbors = neighbors_number(i, j);
                world_next_generation[i][j] = should_live(world[i][j], neighbors);
            }
        }
    }

    return NULL;
}

void init_world() {
    srand(time(NULL));
    for(size_t i = 0; i < ROWS; i++)
        for(size_t j = 0; j < COLS; j++)
            world_next_generation[i][j] = rand() % 2;
}

int neighbors_number(ssize_t row, ssize_t col) {
    int neighbors = 0;

    for(ssize_t i = row - 1; i <= row + 1; i++) {
        for(ssize_t j = col - 1; j <= col + 1; j++) {
            if(is_out_of_bound(i, j)) continue;
            neighbors += world[i][j];
        }
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

void copy_world() {
    memcpy(world, world_next_generation, ROWS * COLS * sizeof(world[0][0]));
}

void draw_world() {
    puts("\033[2J\033[1;1H");
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
