#define _CRT_SECURE_NO_WARNINGS
#include "terminal.h"
#include "platform_utils.h"
#include "game.h"

int main()
{
    srand((unsigned int)time(NULL));
    int rows, cols;
    init(&rows, &cols);
    GameConfig config = {
        .num_stacks = 5,
        .stack_size = 5,
        .num_empty_stacks = 2,
        .show_pulled_list = 1
    };

    int option = run_menu(&rows, &cols);
    while (option == 1) { 
        run_options_menu(rows, cols, &config); // returns 1 if user selected options, then goes to options menu
        clear();
        option = run_menu(&rows, &cols); // After options menu, return to main menu to select start game or exit
	}
    if (option == MENU_NUM - 1) { endwin(); return 0; } // run_menu returns 3 if user exited menu
	getmaxyx(stdscr, rows, cols); // Refresh terminal size after options menu
    run_game(rows, cols, &config);


    endwin();
    return 0;
}