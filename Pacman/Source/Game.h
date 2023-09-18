#pragma once
#include "Radiant.h"

#define SCREEN_WIDTH 600
#define SCREEN_HEIGHT 760

#define PACMAN_SPRITE_WIDTH 35
#define PACMAN_SPEED 250
#define PACMAN_SPAWN_X 300
#define PACMAN_SPAWN_Y 195

#define GHOST_SPRITE_WIDTH 38
#define GHOST_SPEED 150
#define GHOST_HOME_Y 388.89
#define MAX_HOME_Y_RANGE 15

#define BLINKY_HOME_X 301
#define PINKY_HOME_X 301
#define INKY_HOME_X 258
#define CLYDE_HOME_X 344

// the pixel width of a tile
#define TILE_WIDTH 21.5

// width of tile map
#define NUM_TILES_X 32

// height of tile map
#define NUM_TILES_Y 29

// the pixel offset from y = 0.
#define TILE_OFFSET_Y 45
#define TILE_OFFSET_X -32

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

enum PacmanDeathSequence {
	PDS_ShowHit,
	PDS_DeathAnimation,
	PDS_Repawn,
	PDS_NoDeath
};

enum PacmanMessageTypes {
	PMT_PacmanHit,
	PMT_LeaveHome,
	PMT_StartDeathAnimation,
	PMT_PauseGame,
	PMT_ResumeGame,
	PMT_EndDeathAnimation,
	PMT_Respawn
};