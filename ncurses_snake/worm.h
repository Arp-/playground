
#ifndef WORM_WORM_H
#define WORM_WORM_H

#define WORM_BLOCK_W 1
#define WORM_BLOCK_H 1

#include <stdint.h>

//-----------------------------------------------------------------------------//
// STRUCTURES //
typedef enum heading_t {
	NORTH = 0,
	EAST,
	SOUTH,
	WEST,
} heading_t;
//-----------------------------------------------------------------------------//
typedef enum direciton_t {
	FORWARD = 0,
	RIGHT,
	LEFT
} direction_t;
//-----------------------------------------------------------------------------//
typedef struct worm_t {

	struct worm_t* prev_;
	struct worm_t* next_;
	int pos_x_;
	int pos_y_;
	heading_t heading_;

} worm_t;
//-----------------------------------------------------------------------------//
// PUBLIC FUNCTIONS //

// allocates and creates a new worm_t with one node and the given params
worm_t* worm_make(int pos_x, int pos_y, heading_t heading);

/**
 * returns the length of our list
 */
size_t worm_length(const worm_t*);

/*
 * moves the worm according to the param
 * param: the root node of a worm_t list
 * return: the root element of the new worm_t list 
 */
worm_t* worm_move(worm_t*, direction_t);

/**
 * function to handle when the worm eats a block
 * so generally it grows in size, at the end ??
 */
worm_t* worm_eat(worm_t*);

/**
 * frees the whole list
 */
worm_t* worm_destruct(worm_t*);

#endif // WORM_WORM_H
