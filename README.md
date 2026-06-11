# Color Sort — A terminal game written in C

A stack-based color sorting puzzle game that runs in the terminal, built with C and ncurses.

## Gameplay

You have a set of stacks filled with colored blocks. The goal is to sort them so each stack contains only one color. 

- **Left / Right** — move the cursor between stacks
- **Enter** — pick up the top stack of blocks, then place it on another stack
- **C** — cancel and return the held block to its original stack
- **Shift+R** - Shuffle and restart the game with the same configuration.
- **Q** — quit the game

## Installation

### Linux / macOS

Make sure ncurses and make are installed.
Then run `make run` in the source folder

## What is ncurses?

ncurses (new curses) is a C library for building text-based user interfaces (TUIs) that work across a wide variety of terminals. It handles terminal differences automatically and optimizes screen updates to minimize redraw latency.
