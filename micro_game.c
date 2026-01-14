#include <ncurses.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>


int getrandom(int min, int max){
    return min + (int)(rand() * (max - min + 1.0) / (1.0 + RAND_MAX));
}

double normalrandom(double mu, double sigma){
    double sum = 0.0;
    for(int i = 0; i < 12; i++){
        sum += rand() / (double)RAND_MAX;
    }
    return mu + sigma * (sum - 6.0);
}

typedef struct {
    int h, w;
    int num;
    int counter;
    int score;
    int running;
} Game;

typedef struct {
    int x, y;
    int hp;
} Player;

typedef struct {
    int x, y;
    int size;
    int pop;
} Enemy;

typedef struct {
    int x, y;
    int number;
    int pop;
} Item;


void init(void) {
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);

    srand((unsigned int)time(NULL));
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
        //case KEY_UP:    p->y--; break;
        //case KEY_DOWN:  p->y++; break;
        case ERR: break;
    }
}

void update(Game *g, Player *p, Enemy *e, Item *i) {
//--SYSTEM
    if(e->x <= p->x && p->x <= e->x + e->size  &&  e->y == p->y){
        p->hp--;
    }
    g->counter++;

//--PLAYER
    if (p->y < 2) p->y = 2;
    if (p->y >= g->h - 2) p->y = g->h - 3;
    if (p->x < 0) p->x = 0;
    if (p->x >= g->w) p->x = g->w - 1;

//--ENEMY
    if(e->pop){
        if(g->counter == 1){
            e->y++;
        }
    }else{// repop
        e->pop = 1;
        e->y = 0;
        e->x = getrandom(0, g->w - 1);
        e->size = getrandom(4, 6);
        g->score += 10;
    }
    if(e->y == g->h - 3)e->pop = 0;

//--ITEM
    if(i->pop){
        if(g->counter == 2){
            i->y++;
            g->num = normalrandom(0, 8);//
            g->counter = 0;
        }
    }else{// repop
        i->pop = 1;
        i->y = 0;
        //g->num = normalrandom(0, 3);
    }
    if(i->y == g->h - 3)i->pop = 0;
}

void draw(Game *g, Player *p, Enemy *e, Item *i) {
    clear();

    mvaddch(p->y, p->x, '@');
    //mvprintw(i->y, i->x, "(*)");

    if(e->pop){
        switch(e->size){
            case 4:mvprintw(e->y, e->x, "<==>");break;
            case 5:mvprintw(e->y, e->x, "<===>");break;
            case 6:mvprintw(e->y, e->x, "<====>");break;
        }
    }

    if(i->pop){
        double abs_num = fabs(g->num);
        if(abs_num > 5){
            mvprintw(i->y, i->x, "(*)");
        }if(abs_num > 8){
            mvprintw(i->y, i->x, "(@)");
        }else{
            mvprintw(i->y, i->x, "(.)");
        }
    }

    mvprintw(0, 0, "score:%d", g->score);
    mvprintw(1, 0, "HP:%d", p->hp);
    mvprintw(2, 0, "num:%f", fabs(g->num));
    mvhline(3, 0, '#', g->w);

    mvhline(g->h - 2, 0, '#', g->w);
    mvprintw(g->h - 1, 0, "Move with arrows, q to quit");
    refresh();
}


int main(void) {
    Game g = {0, 0, 0, 0, 0, 1};
    Player p = {0, 0, 3};
    Enemy e = {0, 0, 5, 1};
    Item i = {0, 0, 0, 1};

    init();
    timeout(30);   // 最大30msgetch()が待つ

    getmaxyx(stdscr, g.h, g.w);
    p.x = g.w / 2, p.y = g.h - 3;
    e.x = g.w / 2, i.x = g.w / 2 - 3;

    while (g.running) {
        input(&g, &p);
        update(&g, &p, &e, &i);
        draw(&g, &p, &e, &i);
    }
    cleanup();
    printf("SCORE: %d\n", g.score);
    return 0;
}
