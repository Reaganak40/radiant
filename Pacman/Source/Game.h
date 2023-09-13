#pragma once
#include "Radiant.h"

#define SCREEN_WIDTH 600
#define SCREEN_HEIGHT 760

#define PACMAN_SPRITE_WIDTH 28
#define PACMAN_SPEED 260
#define PACMAN_SPAWN_X 300
#define PACMAN_SPAWN_Y 195

#define GHOST_SPRITE_WIDTH 28
#define GHOST_SPEED 190

// the pixel width of a tile
#define TILE_WIDTH 21.5

// width of tile map
#define NUM_TILES_X 32

// height of tile map
#define NUM_TILES_Y 29

// the pixel offset from y = 0.
#define TILE_OFFSET_Y 45
#define TILE_OFFSET_X -30

#define MAP_LAYER 0
#define PAC_DOT_LAYER 1
#define PACMAN_LAYER 2
#define GHOST_LAYER 3

enum PacmanMoveDirection {
	UP = 0,
	DOWN,
	LEFT,
	RIGHT,
	NOMOVE
};