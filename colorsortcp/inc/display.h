#pragma once

#include "platform_utils.h"
#include "stack.h"
#define BG COLOR_BLACK

void draw_box(int row, int col, int height, int width, int color, int pair); 
void draw_stack(int start_row, int col, int box_height, int box_width, Node* head);