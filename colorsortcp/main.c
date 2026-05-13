#define _CRT_SECURE_NO_WARNINGS
#include "terminal.h"
#include "platform_utils.h"
#include "game.h"

int main()
{
    int rows, cols;
    init(&rows, &cols);
    GameConfig config = {
        .num_stacks = 4,
        .stack_size = 4,
        .num_empty = 1
    };

    if (run_menu(rows, cols, &config)) { endwin(); return 0; } // run_menu returns 1 if user exited menu by 'q'
    run_game(rows, cols, &config);

    endwin();
    return 0;
}