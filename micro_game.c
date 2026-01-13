#include <ncurses.h>

typedef struct {
    int x, y;
    int running;
} Game;

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

void update(Game *g) {
    // 最小構成なので空
}

void draw(Game *g) {
    clear();
    mvaddch(g->y, g->x, '@');
    mvprintw(0, 0, "Move with arrows, q to quit");
    refresh();
}

int main(void) {
    Game g = {10, 5, 1};

    init();
    while (g.running) {
        input(&g);
        update(&g);
        draw(&g);
    }
    cleanup();
    return 0;
}
