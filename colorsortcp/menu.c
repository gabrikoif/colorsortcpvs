#include <curses.h>
#include <stdlib.h>

// Adjust these headers based on your actual filenames
#include "menu.h" 
#include "terminal.h"

void draw_options_menu(int selected, GameConfig* config, int rows, int cols)
{
    clear();

    // 1. Draw the Header
    int title_y = rows / 4;
    attron(A_BOLD);
    mvprintw(title_y, (cols - 30) / 2, "==============================");
    mvprintw(title_y + 1, (cols - 30) / 2, "       GAME CONFIGURATION     ");
    mvprintw(title_y + 2, (cols - 30) / 2, "==============================");
    attroff(A_BOLD);

    int menu_y = rows / 2;

    // 2. Option 1: Number of Stacks
    int x1 = (cols - 25) / 2;
    if (selected == 0) {
        attron(A_REVERSE | A_BOLD);
        mvprintw(menu_y, x1, " < Total Tubes: %2d > ", config->num_stacks);
        attroff(A_REVERSE | A_BOLD);
    }
    else {
        mvprintw(menu_y, x1, "   Total Tubes: %2d   ", config->num_stacks);
    }

    // 3. Option 2: Tube Capacity (Max Size)
    int x2 = (cols - 25) / 2;
    if (selected == 1) {
        attron(A_REVERSE | A_BOLD);
        mvprintw(menu_y + 2, x2, " < Tube Height: %2d > ", config->stack_size);
        attroff(A_REVERSE | A_BOLD);
    }
    else {
        mvprintw(menu_y + 2, x2, "   Tube Height: %2d   ", config->stack_size);
    }

    // 4. Option 3: Save & Back Button
    int x3 = (cols - 14) / 2;
    if (selected == 2) {
        attron(A_REVERSE | A_BOLD);
        mvprintw(menu_y + 5, x3, " [ Save & Back ] ");
        attroff(A_REVERSE | A_BOLD);
    }
    else {
        mvprintw(menu_y + 5, x3, "   Save & Back   ");
    }

    // Small footer reminder
    mvprintw(rows - 3, (cols - 36) / 2, "Use Up/Down to navigate, Left/Right to change");
}

// Controls the loop and input state for the Options menu
void run_options_menu(int rows, int cols, GameConfig *config)
{
    int selected = 0;

    while (1) {
        draw_options_menu(selected, config, rows, cols);
        refresh();

        int ch = getch();
        switch (ch) {
        case KEY_UP:
        case 'w':
        case 'W':
            selected = (selected - 1 + 3) % 3;
            break;

        case KEY_DOWN:
        case 's':
        case 'S':
            selected = (selected + 1) % 3;
            break;

        case KEY_LEFT:
        case 'a':
        case 'A':
            if (selected == 0 && config->num_stacks > MIN_NUM_STACKS) {
                config->num_stacks--; // Min bounds check
            }
            else if (selected == 1 && config->stack_size > MIN_STACK_SIZE) {
                config->stack_size--;   // Min bounds check
            }
            break;

        case KEY_RIGHT:
        case 'd':
        case 'D':
            if (selected == 0 && config->num_stacks < MAX_NUM_STACKS) {
                config->num_stacks++; // Max bounds check (adjust to fit screen width)
            }
            else if (selected == 1 && config->stack_size < MAX_STACK_SIZE) {
                config->stack_size++;   // Max bounds check (adjust to fit screen height)
            }
            break;

        case 10: // Enter Key
            if (selected == 2) {
                return; // Exit options loop and go back to main menu
            }
            break;

        case 27: // ESC Key
            return;
        }
    }
}

void draw_menu(int selected, int rows, int cols)
{
    // --- 1. Draw Textured Background safely using Cyan pair 20 ---
    attron(COLOR_PAIR(20) | A_DIM);
    for (int y = 0; y < rows; y++) {
        for (int x = 0; x < cols; x += 2) {
            mvaddch(y, x, '.');
        }
    }
    attroff(COLOR_PAIR(20) | A_DIM);

    // --- 2. Title Card Block ---
    int title_y = rows / 4;
    attron(A_BOLD);
    mvprintw(title_y, (cols - 26) / 2, "==========================");
    mvprintw(title_y + 1, (cols - 26) / 2, "   COLOR SORT ARCADE      ");
    mvprintw(title_y + 2, (cols - 26) / 2, "==========================");
    attroff(A_BOLD);

    // --- 3. Interactive Menu Options ---
    const char* options[] = { "Start Game", "Options", "Exit" };
    int menu_y = rows / 2;

    for (int i = 0; i < 3; i++) {
        int x = (cols - 15) / 2;
        int y = menu_y + (i * 2);

        if (i == selected) {
            attron(A_REVERSE | A_BOLD);
            mvprintw(y, x, " >  %s  < ", options[i]);
            attroff(A_REVERSE | A_BOLD);
        }
        else {
            mvprintw(y, x, "    %s    ", options[i]);
        }
    }
}

int run_menu(int rows, int cols)
{
    // Initialize color pair 20 ONLY for the menu background dots
    if (has_colors()) {
        init_pair(20, COLOR_CYAN, COLOR_BLACK);
    }

    int selected = 0;
    keypad(stdscr, TRUE);
    noecho();
    curs_set(0); // Hide physical blinking terminal cursor

    while (1) {
        draw_menu(selected, rows, cols);
        refresh();

        int ch = getch();
        switch (ch) {
        case KEY_UP:
        case 'w':
        case 'W':
            selected = (selected - 1 + 3) % 3;
            break;
        case KEY_DOWN:
        case 's':
        case 'S':
            selected = (selected + 1) % 3;
            break;
        case 10: // Enter key
            return selected;
        case 27: // ESC key
            return 2; // Treat as exit
		case 'q':
			return 2; // Treat 'q' as exit
        }
    }
}