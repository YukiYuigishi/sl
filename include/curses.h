#pragma once
#include <curses.h>
#include <stdbool.h>

#define TRUE 1
#define FALSE 0
#define OK 0
#define ERR (-1)

typedef struct WINDOW {
  int dummy;
} WINDOW;

extern int COLS;
extern int LINES;
extern WINDOW *stdscr;

WINDOW *initscr(void);
int noecho(void);
int curs_set(int);
int nodelay(WINDOW *stdscr, bool bf);
int leaveok(WINDOW *stdscr, bool bf);
int scrollok(WINDOW *stdscr, bool bf);
int mvaddch(int y, int x, int ch);
int getch(void);
int refresh(void);
int mvcur(int, int, int, int);
int endwin(void);
