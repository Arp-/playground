
#include <stdlib.h>
#include "worm.h"



//-----------------------------------------------------------------------------//
static heading_t movement_vec[][3] = {
	{ NORTH, EAST, WEST }, // NORTH
	{ EAST, SOUTH, NORTH }, // EAST
	{ SOUTH, WEST, EAST }, // SOUTH
	{ WEST, NORTH, SOUTH }  // WEST
};
//-----------------------------------------------------------------------------//
// PRIVATE FUNCTIONS //
static const worm_t* worm_get_tail(const worm_t* w) {
	while (w->prev_ != NULL) {
		w = w->prev_;
	}
	return w;
}
//-----------------------------------------------------------------------------//
static worm_t* worm_get_head(worm_t* w) {
	while (w->next_ != NULL) {
		w = w->next_;
	}
	return w;
}
//-----------------------------------------------------------------------------//
static worm_t* worm_new(worm_t* prev, worm_t* next, int pos_x, int pos_y,
		direction_t dir) {
	worm_t* w = malloc(sizeof(struct worm_t));
	w->prev_ = prev;
	w->next_ = next;
	w->pos_x_ = pos_x;
	w->pos_y_ = pos_y;
	w->heading_ = dir;
	return w;
}
//-----------------------------------------------------------------------------//
static void set_choordinates_next_to(
		worm_t* new_head, int prev_pos_x, int prev_pos_y) {
	switch (new_head->heading_) {
		case NORTH:
			new_head->pos_x_ = prev_pos_x;
			new_head->pos_y_ = prev_pos_y - WORM_BLOCK_H;
			break;
		case EAST:
			new_head->pos_x_ = prev_pos_x + WORM_BLOCK_W;
			new_head->pos_y_ = prev_pos_y;
			break;
		case SOUTH:
			new_head->pos_x_ = prev_pos_x;
			new_head->pos_y_ = prev_pos_y + WORM_BLOCK_H;
			break;
		case WEST:
			new_head->pos_x_ = prev_pos_x - WORM_BLOCK_W;
			new_head->pos_y_ = prev_pos_y;
	}
}
//-----------------------------------------------------------------------------//
// PUBLIC FUNCTIONS //
worm_t* worm_make(int pos_x, int pos_y, heading_t dir) {
	return worm_new(NULL, NULL, pos_x, pos_y, dir);
}
//-----------------------------------------------------------------------------//
size_t worm_length(const worm_t* worm) {
	worm = worm_get_tail(worm);
	size_t i = 0;
	for (i = 0; worm != NULL; worm = worm->next_, ++i);
	return i;
}
//-----------------------------------------------------------------------------//
worm_t* worm_move(worm_t* worm, direction_t way) {
	worm_t* head = worm_get_head(worm);

	// use "trust me i'm an enginner cast" on the new head, becase i'm unfamilliar
	// with c-s const correctness
	worm_t* tail = (worm_t*)worm_get_tail(worm);
	worm_t* new_tail = tail->next_;

	new_tail->prev_ = NULL;
	head->next_ = tail;
	tail->next_ = NULL;
	tail->prev_ = head;
	tail->heading_ = movement_vec[head->heading_][way];
	set_choordinates_next_to(tail, head->pos_x_, head->pos_y_);
	return tail; // The prev tail will be the new head!!!
}
//-----------------------------------------------------------------------------//
worm_t* worm_eat(worm_t* worm) {
	// we just append a node at the end
	worm_t* curr_tail = worm;
	curr_tail = (worm_t*)worm_get_tail(worm);
	curr_tail->prev_ = worm_new(NULL, curr_tail, 
			curr_tail->pos_x_ - WORM_BLOCK_W,
			curr_tail->pos_y_ - WORM_BLOCK_H,
			0);
	// NOTE i generally did the set choordinates, to set the next block not the
	// previous, but if i set the choords using zero, invert it and add the correct
	// choord i should be there
	worm_t* t = curr_tail->prev_;
	set_choordinates_next_to(curr_tail->prev_, 0, 0);
	t->pos_x_ = -(t->pos_x_) + curr_tail->pos_x_;
	t->pos_y_ = -(t->pos_y_) + curr_tail->pos_y_;

	return worm;
}


