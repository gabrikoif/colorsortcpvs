#include "display.h"

void draw_box(int row, int col, int height, int width, int color, int pair)
{
    init_pair(pair, color, color);
    attron(COLOR_PAIR(pair));
    for (int i = row; i < row + height; i++)
        mvhline(i, col, ' ', width);
    attroff(COLOR_PAIR(pair));
}

void draw_stack(int start_row, int col, int box_height, int box_width, Node *head)
{
    int i = 0;
    while (head != NULL)
    {
        int row = start_row + i * box_height;
        draw_box(row, col, box_height, box_width, head->data, head->data);
        head = head->next;
        i++;
    }
}