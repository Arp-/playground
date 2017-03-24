
#include <ncurses.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "worm.h"

#define SPEED 500


typedef struct { 
	int pos_x_;
	int pos_y_;
} food_t;
//-----------------------------------------------------------------------------//
static const worm_t* worm_tail(const worm_t* w) {
	if (w == NULL) { return NULL; }
	while (w->prev_ != NULL) { w = w->prev_; }
	return w;
}
// I could pass the current screen as a paremeter but for now i'll go with
// stdscr
//-----------------------------------------------------------------------------//
static void worm_display(const worm_t* worm) {
	//worm = worm_tail(worm);
	char head_display = '0';
	//switch (worm->heading_) {
	//	case SOUTH:
	//		head_display = '^';
	//		break;
	//	case NORTH:
	//		head_display = 'v';
	//		break;
	//	case WEST:
	//		head_display = '>';
	//		break;
	//	case EAST:
	//		head_display = '<';
	//		break;
	//}
	mvaddch(worm->pos_y_, worm->pos_x_, head_display);
	worm = worm->prev_;
	while (worm != NULL) {
		//printw("pos_y: %d pos_x: %d\n", worm->pos_y_, worm->pos_x_);
		mvaddch(worm->pos_y_, worm->pos_x_, 'O');
		worm = worm->prev_;
	}
}
//-----------------------------------------------------------------------------//
static bool is_on_worm(const worm_t* w, int x, int y) {
	// let's use that the worm_t usually has the header returned everytime
	while (w != NULL) {
		if (w->pos_x_== x && w->pos_y_ == y) {
			return true;
		}
		w = w->prev_;
	}
	return false;
}
//-----------------------------------------------------------------------------//
static void put_food(food_t* f, const worm_t* w, int max_width, int max_height) {
	do {
		// I know that rand is baaaaaad for random generation, but for this simple
		// game this is generally okay.
		f->pos_x_ = rand() % max_width;
		f->pos_y_ = rand() % max_height; 
		move(0,0);
		printw("x: %d, y: %d\n", f->pos_x_, f->pos_y_);
	} while (is_on_worm(w, f->pos_x_, f->pos_y_));
}
//-----------------------------------------------------------------------------//
static bool worm_food_hit_check(const worm_t* w, const food_t* f) {
	return w->pos_x_ == f->pos_x_ && w->pos_y_ == f->pos_y_;
}
//-----------------------------------------------------------------------------//
static void food_display(const food_t* f) {
	mvaddch(f->pos_y_, f->pos_x_, 'f');
}
//-----------------------------------------------------------------------------//
static void food_erase(const food_t* f) {
	mvaddch(f->pos_y_, f->pos_x_, ' ');
}
//-----------------------------------------------------------------------------//
static bool worm_self_hit_check(const worm_t* w) {
	const worm_t* head = w;
	w = w->prev_; // skip the current head
	while (w != NULL) {
		if (w->pos_x_ == head->pos_x_ && w->pos_y_ == head->pos_y_) {
			return true;
		}
		w = w->prev_;
	}
	return false;
}
//-----------------------------------------------------------------------------//
void print_game_over() {
	// TODO make some ascii art ??? // or copy some from somewhere
}
//-----------------------------------------------------------------------------//
int main() {
	srand(time(NULL));
	initscr();
	noecho();
	timeout(SPEED);

	int width;
	int height;
	getmaxyx(stdscr, height, width);
	printw("h: %d | w: %d\n", height, width);



	worm_t* w = worm_make(width/2,height/2, SOUTH);
	w = worm_eat(w);
	w = worm_eat(w);

	food_t f;
	worm_display(w);
	put_food(&f, w, width, height);
	food_display(&f);
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
		if (worm_food_hit_check(w,&f)) {
			w = worm_eat(w);
			food_erase(&f);
			put_food(&f, w, width, height);
		}
		if (worm_self_hit_check(w)) {
			break;
		}
		if (width < w->pos_x_ || w->pos_x_ < 0) { break; }
		if (height < w->pos_y_ || w->pos_y_ < 0) { break; }
		worm_display(w);
		food_display(&f);
		refresh();
	}


	worm_destruct(w);
	print_game_over();
	refresh();
	sleep(1);
	endwin();


	return 0;
}
