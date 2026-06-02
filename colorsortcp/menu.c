#include <curses.h>
#include <stdlib.h>

// Adjust these headers based on your actual filenames
#include "menu.h"
#include "terminal.h"

void draw_options_menu(int selected, GameConfig *config, int rows, int cols)
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
  int opt1 = (cols - 25) / 2;
  if (selected == 0)
  {
    attron(A_REVERSE | A_BOLD);
    mvprintw(menu_y, opt1, " < Total Tubes: %2d > ", config->num_stacks);
    attroff(A_REVERSE | A_BOLD);
  }
  else
  {
    mvprintw(menu_y, opt1, "   Total Tubes: %2d   ", config->num_stacks);
  }

  // 3. Option 2: Tube Capacity (Maopt Size)
  int opt2 = (cols - 25) / 2;
  if (selected == 1)
  {
    attron(A_REVERSE | A_BOLD);
    mvprintw(menu_y + 2, opt2, " < Tube Height: %2d > ", config->stack_size);
    attroff(A_REVERSE | A_BOLD);
  }
  else
  {
    mvprintw(menu_y + 2, opt2, "   Tube Height: %2d   ", config->stack_size);
  }

  int opt3 = (cols - 25) / 2;
  if (selected == 2)
  {
    attron(A_REVERSE | A_BOLD);
    mvprintw(menu_y + 4, opt3, " < Show pulled list: %2d > ", config->show_pulled_list);
    attroff(A_REVERSE | A_BOLD);
  }
  else
  {
    mvprintw(menu_y + 4, opt3, "   Show pulled list: %2d   ", config->show_pulled_list);
  }

  int opt4 = (cols - 25) / 2;
  if (selected == 3)
  {
    attron(A_REVERSE | A_BOLD);
    mvprintw(menu_y + 6, opt4, " < Empty stacks: %2d > ", config->num_empty_stacks);
    attroff(A_REVERSE | A_BOLD);
  }
  else
  {
    mvprintw(menu_y + 6, opt4, "   Empty stacks: %2d   ", config->num_empty_stacks);
  }

  // 4. Option 4: Save & Back Button
  int opt5 = (cols - 14) / 2;
  if (selected == OPT_NUM - 1)
  {
    attron(A_REVERSE | A_BOLD);
    mvprintw(menu_y + 8, opt5, " [ Save & Back ] ");
    attroff(A_REVERSE | A_BOLD);
  }
  else
  {
    mvprintw(menu_y + 8, opt5, "   Save & Back   ");
  }

  // Small footer reminder
  mvprintw(rows - 3, (cols - 36) / 2, "Use Up/Down to navigate, Left/Right to change");
}

// Controls the loop and input state for the Options menu
void run_options_menu(int rows, int cols, GameConfig *config)
{
  int selected = 0;

  while (1)
  {
    draw_options_menu(selected, config, rows, cols);
    refresh();

    int ch = getch();
    switch (ch)
    {
    case KEY_UP:
    case 'w':
    case 'W':
      selected = (selected - 1 + OPT_NUM) % OPT_NUM;
      break;

    case KEY_DOWN:
    case 's':
    case 'S':
      selected = (selected + 1) % OPT_NUM;
      break;

    case KEY_LEFT:
    case 'a':
    case 'A':
      if (selected == 0 && config->num_stacks > MIN_NUM_STACKS)
      {
        config->num_stacks--; // Min bounds check
      }
      else if (selected == 1 && config->stack_size > MIN_STACK_SIZE)
      {
        config->stack_size--; // Min bounds check
      }
      else if (selected == 2)
        config->show_pulled_list = !config->show_pulled_list;
      else if (selected == 3 && config->num_empty_stacks > MIN_EMPTY_STACKS)
      {
        config->num_empty_stacks--;
      }
      break;

    case KEY_RIGHT:
    case 'd':
    case 'D':
      if (selected == 0 && config->num_stacks < MAX_NUM_STACKS)
      {
        config->num_stacks++; // Max bounds check (adjust to fit screen width)
      }
      else if (selected == 1 && config->stack_size < MAX_STACK_SIZE)
      {
        config->stack_size++; // Max bounds check (adjust to fit screen height)
      }
      else if (selected == 2)
        config->show_pulled_list = !config->show_pulled_list;
      else if (selected == 3 && config->num_empty_stacks < MAX_EMPTY_STACKS)
      {
        config->num_empty_stacks++;
      }

      break;

    case 10: // Enter Key
      if (selected == OPT_NUM - 1)
      {
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
  for (int y = 0; y < rows; y++)
  {
    for (int x = 0; x < cols; x += 2)
    {
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
  const char *options[] = {"Start Game", "Options", "Levels", "Refresh Window", "Exit"};
  int menu_y = rows / 2;

  for (int i = 0; i < 5; i++)
  {
    int x = (cols - 15) / 2;
    int y = menu_y + (i * 2);

    if (i == selected)
    {
      attron(A_REVERSE | A_BOLD);
      mvprintw(y, x, " >  %s  < ", options[i]);
      attroff(A_REVERSE | A_BOLD);
    }
    else
    {
      mvprintw(y, x, "    %s    ", options[i]);
    }
  }
}

int run_menu(int *rows, int *cols)
{
  // Initialize color pair 20 ONLY for the menu background dots
  if (has_colors())
  {
    init_pair(20, COLOR_CYAN, COLOR_BLACK);
  }

  int selected = 0;

  while (1)
  {
    draw_menu(selected, *rows, *cols);
    refresh();

    int ch = getch();
    switch (ch)
    {
    case KEY_UP:
    case 'w':
    case 'W':
      selected = (selected - 1 + MENU_NUM) % MENU_NUM;
      break;
    case KEY_DOWN:
    case 's':
    case 'S':
      selected = (selected + 1) % MENU_NUM;
      break;
    case 10: // Enter key
      if (selected == OPT_NUM - 2)
      {
        getmaxyx(stdscr, *rows, *cols);
        clear();
        selected = 0;
        draw_menu(selected, *rows, *cols);
        refresh();
        break;
      }
      else
        return selected;
    case 27:    // ESC key
      return MENU_NUM - 1; // Treat as exit
    case 'q':
      return MENU_NUM - 1; // Treat 'q' as exit
    }
  }
}