// platform_utils.h
#pragma once

#ifdef _WIN32
#include <curses.h>
#include <windows.h>
#else
#include <unistd.h>
#include <ncurses.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <time.h>