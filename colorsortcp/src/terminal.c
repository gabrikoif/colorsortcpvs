#include "terminal.h"
#include "platform_utils.h"

void init(int *rows, int *cols)
{
    initscr();
    curs_set(0);
    start_color();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    getmaxyx(stdscr, *rows, *cols);

    init_pair(COLOR_RED, COLOR_RED, COLOR_RED);
    init_pair(COLOR_BLUE, COLOR_BLUE, COLOR_BLUE);
    init_pair(COLOR_GREEN, COLOR_GREEN, COLOR_GREEN);
    init_pair(COLOR_YELLOW, COLOR_YELLOW, COLOR_YELLOW);
}
