#include <ncurses.h>


typedef struct {
    int h, w;
    int counter;
    int running;
} Game;

typedef struct {
    int x, y;
} Player;

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

void input(Game *g, Player *p) {
    int ch = getch();
    switch (ch) {
        case 'q': g->running = 0; break;
        case KEY_LEFT:  p->x--; break;
        case KEY_RIGHT: p->x++; break;
        case KEY_UP:    p->y--; break;
        case KEY_DOWN:  p->y++; break;
        case ERR: break;
    }
}

void update(Game *g, Player *p, Enemy *e) {
    if (p->y < 2) p->y = 2;
    if (p->y >= g->h - 2) p->y = g->h - 3;

    if (p->x < 0) p->x = 0;
    if (p->x >= g->w) p->x = g->w - 1;

    if(e->pop){
        if(g->counter % 2){
            e->v++;
            e->y = e->v;
        }
    }
    if(e->y == g->h - 3)e->pop = 0;

    g->counter++;
}

void draw(Game *g, Player *p, Enemy *e) {
    clear();
    mvaddch(p->y, p->x, '@');
    if(e->pop)mvprintw(e->y, e->x, "<0w0>");
    mvprintw(0, 0, "count:%d", g->counter);
    mvprintw(1, 0, "HP");
    mvhline(2, 0, '#', g->w);

    mvhline(g->h - 2, 0, '#', g->w);
    mvprintw(g->h - 1, 0, "Move with arrows, q to quit");
    refresh();
}

int main(void) {
    Game g = {0, 0, 0, 1};
    Player p = {0, 0};
    Enemy e = {0, 0, 1, 1};

    init();
    timeout(30);   // 最大30msgetch()が待つ

    getmaxyx(stdscr, g.h, g.w);
    p.x = g.w / 2, p.y = g.h - 3;
    e.x = g.w / 2;

    while (g.running) {
        input(&g, &p);
        update(&g, &p, &e);
        draw(&g, &p, &e);
    }
    cleanup();
    return 0;
}
