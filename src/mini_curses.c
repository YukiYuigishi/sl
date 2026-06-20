#include <curses.h>
#include <stdio.h>

int COLS = 80;
int LINES = 24;

static WINDOW main_window;
WINDOW *stdscr = &main_window;

WINDOW *initscr(void) { return stdscr; };
int noecho(void) { return 0; }
int curs_set(int visibility) { return 0; }
int nodelay(WINDOW *stdscr, bool bf) { return 0; }
int leaveok(WINDOW *stdscr, bool bf) { return 0; }
int scrollok(WINDOW *stdscr, bool bf) { return 0; }
int mvcur(int a, int b, int c, int d) { return 0; }
int endwin(void) { return 0; }
