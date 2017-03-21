
#include <ncurses.h>
#include <unistd.h>
#include "worm.h"



static const worm_t* worm_tail(const worm_t* w) {
	if (w == NULL) { return NULL; }
	while (w->prev_ != NULL) { w = w->prev_; }
	return w;
}
// I could pass the current screen as a paremeter but for now i'll go with
// stdscr
//-----------------------------------------------------------------------------//
static void worm_display(const worm_t* worm) {
	worm = worm_tail(worm);
	while (worm != NULL) {
		//printw("pos_y: %d pos_x: %d\n", worm->pos_y_, worm->pos_x_);
		mvaddch(worm->pos_y_, worm->pos_x_, 'a');
		worm = worm->next_;
	}
}
//-----------------------------------------------------------------------------//
int main() {
	initscr();
	noecho();
	timeout(1000);

	int width;
	int height;
	getmaxyx(stdscr, height, width);
	printw("h: %d | w: %d\n", height, width);


	worm_t* w = worm_make(10, 10, NORTH);
	w = worm_eat(w);
	w = worm_eat(w);
	worm_display(w);
	refresh();

	while (true) { 
		const worm_t* tail = worm_tail(w);
		int prev_x = tail->pos_x_;
		int prev_y = tail->pos_y_;
		int move = getch();
		if (move == 'h') {
			w = worm_move(w, LEFT);
		} else if (move == 'l') {
			w = worm_move(w, RIGHT);
		} else {
			w = worm_move(w, FORWARD); // returns the current head always
		}
		mvaddch(prev_y,prev_x, ' ');
		worm_display(w);
		if (width < w->pos_x_ || w->pos_x_ < 0) { break; }
		if (height < w->pos_y_ || w->pos_y_ < 0) { break; }
		refresh();
	}




	refresh();
	sleep(10);
	endwin();


	return 0;
}
