#pragma once
#include <curses.h>

typedef struct
{
    int num_stacks;
    int stack_size;
    int num_empty;
} GameConfig;

#define MIN_NUM_STACKS 2
#define MAX_NUM_STACKS 6
#define MIN_STACK_SIZE 2
#define MAX_STACK_SIZE 10

int run_menu(int rows, int cols);
void run_options_menu(int rows, int cols, GameConfig* config);