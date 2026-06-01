#pragma once
#include <curses.h>

#define OPT_NUM 5
#define MENU_NUM 4
typedef struct
{
    int num_stacks;
    int stack_size;
    int num_empty_stacks;
    int show_pulled_list; // Boolean
} GameConfig;

#define MIN_NUM_STACKS 2
#define MAX_NUM_STACKS 6
#define MIN_STACK_SIZE 2
#define MAX_STACK_SIZE 10
#define MIN_EMPTY_STACKS 2
#define MAX_EMPTY_STACKS 4

int run_menu(int rows, int cols);
void run_options_menu(int rows, int cols, GameConfig* config);