#include "game.h"
#include "display.h"
#include "terminal.h"
#include "platform_utils.h"
#include "stack.h"

static int NUM_STACKS;
static int MAX_SIZE;
static int NUM_EMPTY;
static int SHOW_PULLED_LIST;

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

static void draw_all(int top_row, int start_col, Node** stacks, Node* pulledList, int selected, int rows)
{
    clear();

    // 1. Draw a solid floor across the entire width of the terminal
    int floor_row = top_row + (BOX_H * MAX_SIZE);
    attron(A_NORMAL | A_BOLD);
    mvhline(floor_row, 0, ACS_HLINE, COLS);
    attroff(A_NORMAL | A_BOLD);

    for (int i = 0; i < NUM_STACKS; i++)
    {
        // Increased spacing between tubes (swapped +2 for +4)
        int col = start_col + i * (BOX_W + 4);

        // 2. The New, Higher Cursor
        if (i == selected)
        {
            attron(A_BOLD);
            // Pushed way higher (-4 instead of -1) to hover nicely
            mvprintw(top_row - 4, col + (BOX_W / 2) - 1, " v ");
            attroff(A_BOLD);
        }

        // 3. Draw the Flared Test Tube Lip
        // This adds little outward-curving edges to the top of the glass
        attron(A_NORMAL);
        mvaddch(top_row - 1, col - 2, ACS_ULCORNER);          // Flare left
        mvhline(top_row - 1, col - 1, ACS_HLINE, 1);
        mvhline(top_row - 1, col + BOX_W, ACS_HLINE, 1);
        mvaddch(top_row - 1, col + BOX_W + 1, ACS_URCORNER);  // Flare right
        attroff(A_NORMAL);

        // 4. Draw the actual stack
        draw_stack(top_row, col, BOX_H, BOX_W, stacks[i], MAX_SIZE);

        // 5. Connect the bottom of the tube to the floor
        attron(A_NORMAL);
        mvaddch(floor_row, col - 1, ACS_BTEE);     // T-junction left
        mvaddch(floor_row, col + BOX_W, ACS_BTEE); // T-junction right
        attroff(A_NORMAL);
    }

    draw_stack(top_row, start_col - BOX_W * 3, BOX_H, BOX_W, pulledList, MAX_SIZE);

    mvprintw(rows - 2, start_col, "Enter/Up/Down to push or pull, c to cancel, shift + r to shuffle again, q to quit");

    refresh();
}

static void init_stacks(Node **stacks)
{
    int num_color_stacks = NUM_STACKS - NUM_EMPTY;
    int total = num_color_stacks * MAX_SIZE;

    int *colors = malloc(total * sizeof(int));
    if (colors == NULL)
        return;

    int pallete[] = {
    PAIR_RED,
    PAIR_GREEN,
    PAIR_YELLOW,
    PAIR_BLUE,
    PAIR_MAGENTA,
    PAIR_CYAN,
    };

	Node* prev_colors = NULL; // To keep track of previously generated colors for each stack
    for (int i = 0; i < num_color_stacks; i++) {
        int palette_size = sizeof(pallete) / sizeof(pallete[0]);
        int random_color = pallete[rand() % palette_size];

        while (in_stack(prev_colors, random_color)) {
            random_color = pallete[rand() % palette_size];
        }

        // Push the unique color to your tracking stack
        push(&prev_colors, random_color);
        for (int j = 0; j < MAX_SIZE; j++) {
            colors[i * MAX_SIZE + j] = random_color;
        }
    } 


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
	free_list(prev_colors);
}

void run_game(int rows, int cols, GameConfig *config)
{
    NUM_STACKS = config->num_stacks + config->num_empty_stacks;
    MAX_SIZE = config->stack_size;
    NUM_EMPTY = config->num_empty_stacks;
    SHOW_PULLED_LIST = config->show_pulled_list;

    Node **stacks = malloc(NUM_STACKS * sizeof(Node *)); // Array of stacks(size of stacks) aka stacks[NUM_STACKS].
    // Did this to avoid VLA problems.
    if (stacks == NULL)
        return;
    init_stacks(stacks);

    Node *pulledList = NULL;
    // Empty stack for pulled values(colors).

    int total_width = NUM_STACKS * (BOX_W + 2);
    int start_col = (cols - total_width) / 2;
    int total_height = BOX_H * MAX_SIZE;
    int bottom_row = (rows / 2) - (total_height / 2);

    int selected = 0;
    int source = -1; // What stack the color was picked up from.
    int move_counter = 0;

    draw_all(bottom_row, start_col, stacks, pulledList, selected, rows);

    int ch;
    while ((ch = getch()) != 'q')
    {
        if (ch == 'R')
        {
            getmaxyx(stdscr, rows, cols);
            total_width = NUM_STACKS * (BOX_W + 2);
            start_col = (cols - total_width) / 2;
            total_height = BOX_H * MAX_SIZE;
            bottom_row = (rows / 2) - (total_height / 2);
			init_stacks(stacks);
            clear();
            draw_all(bottom_row, start_col, stacks, pulledList, selected, rows);
        }
        int pulledListSize = list_size(pulledList);
        switch (ch)
        {
        case KEY_LEFT:
            selected = (selected - 1 + NUM_STACKS) % NUM_STACKS;
            break;

        case KEY_RIGHT:
            selected = (selected + 1) % NUM_STACKS;
            break;

        case KEY_UP:
        case KEY_DOWN:
        case KEY_MOUSE:
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
                if (stacks[selected] != NULL && pulledList->data != stacks[selected]->data)
                // Compare top color in color stack and color of memory stack.
                // If they are different, don't let the push.
                {
                    // Add message here
                    break;
                }
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

        draw_all(bottom_row, start_col, stacks, pulledList, selected, rows);

        if (check_win(stacks))
        {
            mvprintw(rows/2 - 6, cols / 2 - 6, "YOU WIN! Took you %d moves", move_counter);
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