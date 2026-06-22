#include <curses.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#ifdef __Unikraft__
#include <uk/config.h>
#include <uk/console.h>
#include <uk/plat/time.h>
#if CONFIG_HAVE_SCHED
#include <uk/sched.h>
#endif
#endif

#define SCREEN_COLS 80
#define SCREEN_LINES 24
#define FRAME_SIZE (3 + SCREEN_LINES * SCREEN_COLS + \
                    (SCREEN_LINES - 1) * 2 + 1)

int COLS = SCREEN_COLS;
int LINES = SCREEN_LINES;

static WINDOW main_window;
static char screen[SCREEN_LINES][SCREEN_COLS];
static char frame[FRAME_SIZE];
static int cursor_visibility = 1;
WINDOW *stdscr = &main_window;

WINDOW *initscr(void)
{
    memset(screen, ' ', sizeof(screen));
    printf("\033[2J\033[H");
    return stdscr;
}

int noecho(void) { return OK; }

int curs_set(int visibility)
{
    int previous = cursor_visibility;

    cursor_visibility = visibility;
    printf(visibility ? "\033[?25h" : "\033[?25l");
    return previous;
}

int nodelay(WINDOW *win, bool bf)
{
    (void)win;
    (void)bf;
    return OK;
}

int leaveok(WINDOW *win, bool bf)
{
    (void)win;
    (void)bf;
    return OK;
}

int scrollok(WINDOW *win, bool bf)
{
    (void)win;
    (void)bf;
    return OK;
}

int mvaddch(int y, int x, int ch)
{
    if (y < 0 || y >= LINES || x < 0 || x >= COLS)
        return ERR;

    screen[y][x] = (char)ch;
    return OK;
}

int getch(void) { return ERR; }

int refresh(void)
{
    char *p = frame;
    int y;

    *p++ = '\033';
    *p++ = '[';
    *p++ = 'H';
    for (y = 0; y < LINES; ++y) {
        memcpy(p, screen[y], COLS);
        p += COLS;
        if (y != LINES - 1) {
            *p++ = '\r';
            *p++ = '\n';
        }
    }
    *p = '\0';

    return uk_console_out(frame, (unsigned int)(p - frame)) == p - frame
           ? OK : ERR;
}

#ifdef __Unikraft__
int usleep(useconds_t usec)
{
    __nsec nsec = (__nsec)usec * 1000ULL;

#if CONFIG_HAVE_SCHED
    uk_sched_thread_sleep(nsec);
#else
    __nsec until = ukplat_monotonic_clock() + nsec;

    while (ukplat_monotonic_clock() < until)
        ;
#endif
    return 0;
}
#endif

int mvcur(int old_y, int old_x, int new_y, int new_x)
{
    (void)old_y;
    (void)old_x;

    if (new_y < 0 || new_y >= LINES || new_x < 0 || new_x >= COLS)
        return ERR;

    return printf("\033[%d;%dH", new_y + 1, new_x + 1) < 0 ? ERR : OK;
}

int endwin(void)
{
    printf("\033[?25h");
    cursor_visibility = 1;
    return OK;
}
