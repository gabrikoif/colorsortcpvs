#pragma once

typedef struct
{
    int num_stacks;
    int stack_size;
    int num_empty;
} GameConfig;

int run_menu(int rows, int cols, GameConfig* config);