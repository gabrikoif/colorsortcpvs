#include "display.h"
#include "terminal.h"

static int color_to_pair(int color)
{
    switch (color)
    {
    case COLOR_RED:     return PAIR_RED;
    case COLOR_BLUE:    return PAIR_BLUE;
    case COLOR_GREEN:   return PAIR_GREEN;
    case COLOR_YELLOW:  return PAIR_YELLOW;
    case COLOR_CYAN:    return PAIR_CYAN;
    case COLOR_MAGENTA: return PAIR_MAGENTA;
    default:            return 0;
    }
}

void draw_box(int row, int col, int height, int width, int color)
{
    int pair = color_to_pair(color);
    attron(COLOR_PAIR(pair));
    for (int i = row; i < row + height; i++)
        mvhline(i, col, ' ', width);
    attroff(COLOR_PAIR(pair));
}

void draw_stack(int start_row, int col, int box_height, int box_width, Node* head)
{
    int i = 0;
    while (head != NULL)
    {
        int row = start_row + i * box_height;
        draw_box(row, col, box_height, box_width, head->data);
        head = head->next;
        i++;
    }
}