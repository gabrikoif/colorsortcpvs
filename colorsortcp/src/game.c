#include "game.h"
#include "display.h"
#include "terminal.h"
#include "platform_utils.h"
#include "stack.h"

static int NUM_STACKS;
static int MAX_SIZE;
static int NUM_EMPTY;

static int stack_solved(Node *head)
{
    if (list_size(head) != MAX_SIZE)
        return 0;
    int color = head->data;
    Node *cur = head->next;
    while (cur != NULL)
    {
        if (cur->data != color)
            return 0;
        cur = cur->next;
    }
    return 1;
}

static int check_win(Node **stacks)
{
    for (int i = 0; i < NUM_STACKS; i++)
        if (stacks[i] != NULL && !stack_solved(stacks[i]))
            return 0;
    return 1;
}

static void draw_all(int bottom_row, int start_col, Node **stacks,
                     int selected, int held, int rows)
{
    clear();
    for (int i = 0; i < NUM_STACKS; i++)
    {
        int col = start_col + i * (BOX_W + 2);
        int len = list_size(stacks[i]);

        if (i == selected)
            mvprintw(bottom_row - BOX_H * MAX_SIZE - 1, col + BOX_W / 2 - 1, "[^]");

        if (len > 0)
        {
            int row = bottom_row - BOX_H * (len - 1);
            draw_stack(row, col, BOX_H, BOX_W, stacks[i]);
        }
    }

    if (held != -1)
        mvprintw(rows - 2, start_col, "Holding color %d  (c to cancel)", held);
    else
        mvprintw(rows - 2, start_col, "Left/Right to move, Enter to pick up, q to quit");

    refresh();
}

static void init_stacks(Node **stacks)
{
    int num_color_stacks = NUM_STACKS - NUM_EMPTY;
    int total = num_color_stacks * MAX_SIZE;

    // Build flat array of all colors
    int colors[total];
    for (int i = 0; i < num_color_stacks; i++)
        for (int j = 0; j < MAX_SIZE; j++)
            colors[i * MAX_SIZE + j] = COLOR_RED + (i % 6); // Specific color at index i to i + MAX_SIZE - 1
                                                            // eg: 0 -> MAX_SIZE - 1: RED

    // Fisher-Yates shuffle
    srand(time(NULL));
    for (int i = total - 1; i > 0; i--)
    {
        int j = rand() % (i + 1);
        int tmp = colors[i];
        colors[i] = colors[j];
        colors[j] = tmp;
    }

    // Deal into stacks
    int idx = 0;
    for (int i = 0; i < num_color_stacks; i++)
    {
        stacks[i] = NULL;
        for (int j = 0; j < MAX_SIZE; j++)
            push(&stacks[i], colors[idx++]);
    }

    // Empty buffer stacks
    for (int i = num_color_stacks; i < NUM_STACKS; i++)
        stacks[i] = NULL;
}

void run_game(int rows, int cols, GameConfig *config)
{
    NUM_STACKS = config->num_stacks + config->empty_stacks;
    MAX_SIZE = config->stack_size;
    NUM_EMPTY = config->empty_stacks;

    Node *stacks[NUM_STACKS];
    init_stacks(stacks);

    int total_width = NUM_STACKS * (BOX_W + 2);
    int start_col = (cols - total_width) / 2;
    int bottom_row = (rows / 2) + (BOX_H * MAX_SIZE) / 2 - BOX_H;

    int selected = 0;
    int held = -1;   // What color is picked up. -1 means nothing.
    int source = -1; // What stack the color was picked up from.
    int move_counter = 0;

    draw_all(bottom_row, start_col, stacks, selected, held, rows);

    int ch;
    while ((ch = getch()) != 'q')
    {
        switch (ch)
        {
        case KEY_LEFT:
            selected = (selected - 1 + NUM_STACKS) % NUM_STACKS;
            break;

        case KEY_RIGHT:
            selected = (selected + 1) % NUM_STACKS;
            break;

        case '\n':
        case KEY_ENTER:
            if (held == -1)
            {
                int val;
                if (pull(&stacks[selected], &val) == 0)
                {
                    held = val;
                    source = selected;
                }
            }
            else
            {
                if (list_size(stacks[selected]) < MAX_SIZE)
                {
                    push(&stacks[selected], held);
                    if (selected != source) { move_counter++; } 
                    // Only if user pushed to other stack from which selected the move counter increments by 1.
                    held = -1;
                    source = -1;
                }
            }
            break;

        case 'c':
            if (held != -1)
            {
                push(&stacks[source], held);
                held = -1;
                source = -1;
            }
            break;
        }

        draw_all(bottom_row, start_col, stacks, selected, held, rows);

        if (check_win(stacks))
        {
            mvprintw(rows / 2, cols / 2 - 4, "YOU WIN! Took you %d moves", move_counter);
            refresh();
            getch();
            break;
        }
    }

    if (held != -1)
    {
        push(&stacks[source], held);
    }

    for (int i = 0; i < NUM_STACKS; i++)
        free_list(stacks[i]);
}