// platform_utils.h
#pragma once

#ifdef _WIN32
    #include <curses.h>
    #include <windows.h>
#else
    #include <unistd.h>
    #include <ncurses.h>
#endif
#include <stdlib.h>
#include <time.h>

// Cross-platform sleep (milliseconds)
static inline void cSleep(int milliseconds) {
#ifdef _WIN32
    Sleep(milliseconds);
#else
    sleep(milliseconds / 1000); // sleep(int) takes seconds instead of millliseconds.
#endif
}