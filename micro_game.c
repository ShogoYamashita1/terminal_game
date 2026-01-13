#include <ncurses.h>


typedef struct {
    int x, y;
    int h, w;
    int running;
} Game;

typedef struct {
    int x, y;
    int v;
    int pop;
} Enemy;


void init(void) {
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);
}

void cleanup(void) {
    endwin();
}

void input(Game *g) {
    int ch = getch();
    switch (ch) {
        case 'q': g->running = 0; break;
        case KEY_LEFT:  g->x--; break;
        case KEY_RIGHT: g->x++; break;
        case KEY_UP:    g->y--; break;
        case KEY_DOWN:  g->y++; break;
    }
}

void update(Game *g, Enemy *e) {
    if (g->y < 2) g->y = 2;
    if (g->y >= g->h - 2) g->y = g->h - 3;

    if (g->x < 0) g->x = 0;
    if (g->x >= g->w) g->x = g->w - 1;

    if(e->pop){
        e->v++;
        e->y = e->v;
    }
    if(e->y == g->h - 3)e->pop = 0;
}

void draw(Game *g, Enemy *e) {
    clear();
    mvaddch(g->y, g->x, '@');
    if(e->pop)mvprintw(e->y, e->x, "<0w0>");
    mvprintw(0, 0, "HP");
    mvprintw(1, 0, "#######################################");
    mvprintw(g->h - 2, 0, "################################");
    mvprintw(g->h - 1, 0, "Move with arrows, q to quit");
    refresh();
}

int main(void) {
    Game g = {0, 0, 0, 0, 1};
    Enemy e = {0, 0, 1, 1};

    init();
    getmaxyx(stdscr, g.h, g.w);
    g.x = g.w / 2, g.y = g.h - 3;
    e.x = g.w / 2;

    while (g.running) {
        input(&g);
        update(&g, &e);
        draw(&g, &e);
    }
    cleanup();
    return 0;
}
