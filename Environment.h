#pragma once
#ifndef _ENVIRONMENT_H
#define _ENVIRONMENT_H


#include "stdafx.h"
enum class GameState {
	PLAYING, PAUSED, WIN, LOSE, BLOCK_MOVING 
};

#define BLOCK_HORIZONTAL 7
#define BLOCK_VERTICAL 9
#define BLOCK_OFFSET Vec2(720/2-220, 1280/2-300);
#define BLOCK_WIDTH 80
#define BLOCK_HEIGHT 80
#define BLOCK_VAR 4 // a number of block types
#define SWAP(TYPE, A, B){TYPE t=A; A=B; B=t;}

#define FONT_NAME "fonts/SDSamliphopangcheTTFBasic.ttf"

class Global {
private:
	Global(){}
	long long score = 0;
	int backgoundMusicId;
public:
	static Global* getInstance();

	void setScore(long long value);
	long long getScore();
	void addScore(long long value);

	void playPop();
	void playBackgroundMusic();
	void stopMackgroundMusic();
	void blockPopMusic();
};
#endif