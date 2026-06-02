#include "display.h"
#include "terminal.h" // Ensures your PAIR_ definitions are accessible
#include <curses.h>

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

// Draws an empty glass segment at the top of the tube
static void draw_empty_box(int row, int col, int height, int width, int draw_tube)
{
    for (int i = 0; i < height; i++) {
      if (draw_tube)
      {
        attron(A_NORMAL);
        mvaddch(row + i, col - 1, ACS_VLINE);        // Left glass wall
        mvaddch(row + i, col + width, ACS_VLINE);    // Right glass wall
        attroff(A_NORMAL);
      }

        // Clears out the inside space cleanly
        mvhline(row + i, col, ' ', width);
    }
}

// Draws a filled liquid segment inside the glass tube
static void draw_filled_box(int row, int col, int height, int width, int color, int draw_tube)
{
    int pair = color_to_pair(color);
    for (int i = 0; i < height; i++) {
      if (draw_tube)
      {
        attron(A_NORMAL);
        mvaddch(row + i, col - 1, ACS_VLINE);
        mvaddch(row + i, col + width, ACS_VLINE);
        attroff(A_NORMAL);
      }

        // Draws the colorful liquid inside
        attron(COLOR_PAIR(pair));
        mvhline(row + i, col, ' ', width);
        attroff(COLOR_PAIR(pair));
    }
}

// Draws the rounded bottom of the test tube
static void draw_tube_bottom(int row, int col, int width)
{
    attron(A_NORMAL);
    mvaddch(row, col - 1, ACS_LLCORNER);
    mvhline(row, col, ACS_HLINE, width);
    mvaddch(row, col + width, ACS_LRCORNER);
    attroff(A_NORMAL);
}

// Always draws to stack_size so the physical tubes never shrink or move
void draw_stack(int start_row, int col, int box_height, int box_width, Node* head, int stack_size, int draw_tube)
{
    int current_items = 0;
    Node* temp = head;
    while (temp != NULL) {
        current_items++;
        temp = temp->next;
    }

    int empty_slots = stack_size - current_items;
    temp = head;

    for (int i = 0; i < stack_size; i++) {
        int row = start_row + i * box_height;

        if (i < empty_slots) {
            draw_empty_box(row, col, box_height, box_width, draw_tube);
        }
        else {
            draw_filled_box(row, col, box_height, box_width, temp->data, draw_tube);
            temp = temp->next;
        }
    }

    int bottom_row = start_row + stack_size * box_height;
    if (draw_tube) draw_tube_bottom(bottom_row, col, box_width);
    
}