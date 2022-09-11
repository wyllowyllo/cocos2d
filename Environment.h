#pragma once
#ifndef _ENVIRONMENT_H
#define _ENVIRONMENT_H


#include "stdafx.h"
enum class GameState {
	PLAYING, PAUSED, WIN, LOSE, BLOCK_MOVING
};

#define BLOCK_HORIZONTAL 7
#define BLOCK_VERTICAL 9
#define BLOCK_OFFSET Vec2(720/2-200, 1280/2-300);
#define BLOCK_WIDTH 80
#define BLOCK_HEIGHT 80

#define SWAP(TYPE, A, B){TYPE t=A; A=B; B=t;}
#endif