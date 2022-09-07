#pragma once
#ifndef _SCENEINGAME_H
#define _SCENEINGAME_H

#include "stdafx.h"
#include "Environment.h"
#endif

class SceneInGame :public Scene {
private:
	GameState gamestate;

	// if blockdata value is zero, it's empty block if value is a positive number(not zero) it's a block
	int BlockData[BLOCK_VERTICAL][BLOCK_HORIZONTAL];

	//if blocksprite is nullptr, it's empty
	Sprite* BlockSprite[BLOCK_VERTICAL][BLOCK_HORIZONTAL];

	//유니크 스택을 위한 자료구조
	Vec2 judgeStack[128];
	//스택에 들어있는 자료의 수
	int judgeStackCount = 0;
	//0이라면 스택에 자료가 없음, 그게 아니라면 자료가 있음
	int judgeData[BLOCK_VERTICAL][BLOCK_HORIZONTAL];

	void createBlock(int x, int y, int type);
	int getBlockData(int x, int y);
	void setBlockData(int x, int y, int type);
	Sprite* getBlockSprite(int x, int y);
	void setBlockSprite(int x, int y, Sprite* s);
	void destroyBlcok(int x, int y);

	Vec2 ConvertGameCoordToBlockCoord(Vec2 Gamecoord);
	Vec2 ConvertBlockCoordToGameCoord(Vec2 Blockcoord);

	// 아래에서부터 찾아 올라가면서 비어있는 블록을 찾고
	//-1이 리턴이 되면 비어있는 블록이 없다는 뜻

	int findEmptyBlockIndex(int x, int y);

	//y위치부터 찾아 올라가면서 비어있지 않은 블록을 찾고
	//-1이 리턴이 되면 비어있지 않은 블록이 없다는 뜻이다.
	int findFilledBlockIndex(int x, int y);
	//블록 떨어뜨리는 함수
	void DropBlock(int x);

	void stackPush(Vec2 value);
	Vec2 stackPop();
	void stackEmpty();
	bool stackFind(Vec2 value);
	void judgeMatch(int x, int y);
public:
	static SceneInGame* create();
	virtual bool init() override;
	virtual void onEnter() override;

	void initUI();;
	void initGame();
	void destroyUI();
	void destroyGame();
	void alignBlcokSprite();

	bool onTouchBegan(Touch* t, Event* e);
	void onTouchMoved(Touch* t, Event* e);
	void onTouchEnded(Touch* t, Event* e);


	void StartGame();
	void PauseGame();
	void WinGame();
	void LoseGame();

};