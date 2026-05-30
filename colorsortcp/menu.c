#include <curses.h>
#include <stdlib.h>

// Adjust these headers based on your actual filenames
#include "menu.h" 
#include "terminal.h" 

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
        }
    }
}