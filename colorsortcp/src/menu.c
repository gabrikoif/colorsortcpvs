#include "menu.h"
#include "platform_utils.h"

#define NUM_ITEMS 4 // 3 settings + Start

#define MIN_STACKS 2
#define MAX_STACKS 8
#define MIN_SIZE 3
#define MAX_SIZE 6
#define MIN_EMPTY 1
#define MAX_EMPTY 3

static const char *labels[] = {
    "Number of Stacks",
    "Stack Size",
    "Empty Stacks (Difficulty)",
    "Start",
    "NOTE: IF YOU PICK UP AND PRESS ENTER TO PUT DOWN IT COUNTS AS A MOVE. PRESS C TO CANCEL"
};

static void draw_menu(int rows, int cols, GameConfig *config,
                      int cursor, int editing)
{
    clear();

    int values[3] = {
        config->num_stacks,
        config->stack_size,
        config->empty_stacks};

    int start_row = rows / 2 - NUM_ITEMS;

    mvprintw(start_row - 2, cols / 2 - 5, "COLOR SORT");

    for (int i = 0; i < NUM_ITEMS; i++)
    {
        int row = start_row + i * 2;
        int is_selected = (i == cursor);
        int is_editing = (i == cursor && editing);

        if (i == NUM_ITEMS - 1)
        {
            // Start button
            if (is_selected)
                mvprintw(row, cols / 2 - 3, "> START <");
            else
                mvprintw(row, cols / 2 - 3, "  START  ");
        }
        else
        {
            if (is_editing)
                mvprintw(row, cols / 2 - 15, "  %s:  < %d >", labels[i], values[i]);
            else if (is_selected)
                mvprintw(row, cols / 2 - 15, "> %s:    %d  ", labels[i], values[i]);
            else
                mvprintw(row, cols / 2 - 15, "  %s:    %d  ", labels[i], values[i]);
        }
    }

    mvprintw(rows - 2, cols / 2 - 20,
             "Up/Down: navigate  Enter: select  Esc: back  q: quit");

    refresh();
}

int run_menu(int rows, int cols, GameConfig *config)
{
    int cursor = 0;
    int editing = 0;

    draw_menu(rows, cols, config, cursor, editing);

    int ch;
    while ((ch = getch()) != 'q')
    {
        if (editing)
        {
            switch (ch)
            {
            case KEY_LEFT:
                if (cursor == 0 && config->num_stacks > MIN_STACKS)
                    config->num_stacks--;
                else if (cursor == 1 && config->stack_size > MIN_SIZE)
                    config->stack_size--;
                else if (cursor == 2 && config->empty_stacks > MIN_EMPTY)
                    config->empty_stacks--;
                break;

            case KEY_RIGHT:
                if (cursor == 0 && config->num_stacks < MAX_STACKS)
                    config->num_stacks++;
                else if (cursor == 1 && config->stack_size < MAX_SIZE)
                    config->stack_size++;
                else if (cursor == 2 && config->empty_stacks < MAX_EMPTY)
                    config->empty_stacks++;
                break;

            case '\n':
            case KEY_ENTER:
            case 27: // Escape
                editing = 0;
                break;
            }
        }
        else
        {
            switch (ch)
            {
            case KEY_UP:
                cursor = (cursor - 1 + NUM_ITEMS) % NUM_ITEMS;
                break;

            case KEY_DOWN:
                cursor = (cursor + 1) % NUM_ITEMS;
                break;

            case '\n':
            case KEY_ENTER:
                if (cursor == NUM_ITEMS - 1)
                    return 0; // Start game
                else
                    editing = 1;
                break;
            }
        }

        draw_menu(rows, cols, config, cursor, editing);
    }

    return 1;
}