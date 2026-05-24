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
                     int selected, int rows)
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
    mvprintw(rows - 2, start_col, "Left/Right to move, Enter to pick up, q to quit");

    refresh();
}

static void init_stacks(Node **stacks)
{
    int num_color_stacks = NUM_STACKS - NUM_EMPTY;
    int total = num_color_stacks * MAX_SIZE;

    int *colors = malloc(total * sizeof(int));
    if (colors == NULL)
        return;

    const int pallete[] = {
        PAIR_RED, PAIR_BLUE, PAIR_GREEN, PAIR_YELLOW, PAIR_CYAN, PAIR_MAGENTA};

    for (int i = 0; i < num_color_stacks; i++)
        for (int j = 0; j < MAX_SIZE; j++)
            colors[i * MAX_SIZE + j] = pallete[i % 6];

    srand(time(NULL));
    for (int i = total - 1; i > 0; i--)
    {
        int j = rand() % (i + 1);
        int tmp = colors[i];
        colors[i] = colors[j];
        colors[j] = tmp;
    }

    int idx = 0;
    for (int i = 0; i < num_color_stacks; i++)
    {
        stacks[i] = NULL;
        for (int j = 0; j < MAX_SIZE; j++)
            push(&stacks[i], colors[idx++]);
    }

    for (int i = num_color_stacks; i < NUM_STACKS; i++)
        stacks[i] = NULL;

    free(colors);
}

void run_game(int rows, int cols, GameConfig *config)
{
    NUM_STACKS = config->num_stacks + config->num_empty;
    MAX_SIZE = config->stack_size;
    NUM_EMPTY = config->num_empty;

    Node **stacks = malloc(NUM_STACKS * sizeof(Node *)); // Array of stacks(size of stacks) aka stacks[NUM_STACKS].
    // Did this to avoid VLA problems.
    if (stacks == NULL)
        return;
    init_stacks(stacks);

    Node *pulledList = NULL;
    // Empty stack for pulled values(colors).

    int total_width = NUM_STACKS * (BOX_W + 2);
    int start_col = (cols - total_width) / 2;
    int bottom_row = (rows / 2) + (BOX_H * MAX_SIZE) / 2 - BOX_H;

    int selected = 0;
    int source = -1; // What stack the color was picked up from.
    int move_counter = 0;

    draw_all(bottom_row, start_col, stacks, selected, rows);

    int ch;
    while ((ch = getch()) != 'q')
    {
        int pulledListSize = list_size(pulledList);
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
            if (pulledListSize == 0)
            {
                int val;
                if (pull(&stacks[selected], &val) == 0)
                {
                    push(&pulledList, val);
                    source = selected;
                }
                // Initial pull of one color from color stack.
                // If next is the same color, pull that one too.
                while (stacks[selected] != NULL && val == stacks[selected]->data)
                {
                    if (pull(&stacks[selected], &val) == 0)
                    {
                        push(&pulledList, val);
                    }
                }
            }
            else
            {
                if (list_size(stacks[selected]) + list_size(pulledList) <= MAX_SIZE)
                {
                    int valToPush;
                    for (int i = 0; i < pulledListSize; i++)
                    {
                        pull(&pulledList, &valToPush);      // Pull from hidden memory stack
                        push(&stacks[selected], valToPush); // Push back to color stack.
                    }
                    if (selected != source)
                    {
                        move_counter++;
                    }
                    // Only if user pushed to other stack from which selected the move counter increments by 1.
                    source = -1;
                }
            }
            break;

        case 'c':
            if (pulledListSize != 0)
            {
                int valToPush;
                for (int i = 0; i < pulledListSize; i++)
                {
                    pull(&pulledList, &valToPush);    // Pull from hidden memory stack
                    push(&stacks[source], valToPush); // Push back to color stack.
                }
            }
            source = -1;
            break;
        }

        draw_all(bottom_row, start_col, stacks, selected, rows);

        if (check_win(stacks))
        {
            mvprintw(rows / 2, cols / 2 - 4, "YOU WIN! Took you %d moves", move_counter);
            refresh();
            getch();
            break;
        }
    }

    if (list_size(pulledList) != 0)
    {
        int valToPush;
        for (int i = 0; i < list_size(pulledList); i++)
        {
            pull(&pulledList, &valToPush);    // Pull from hidden memory stack
            push(&stacks[source], valToPush); // Push back to color stack.
        }
    }

    for (int i = 0; i < NUM_STACKS; i++)
        free_list(stacks[i]);
    free(stacks);
}