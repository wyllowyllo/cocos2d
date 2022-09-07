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

	//����ũ ������ ���� �ڷᱸ��
	Vec2 judgeStack[128];
	//���ÿ� ����ִ� �ڷ��� ��
	int judgeStackCount = 0;
	//0�̶�� ���ÿ� �ڷᰡ ����, �װ� �ƴ϶�� �ڷᰡ ����
	int judgeData[BLOCK_VERTICAL][BLOCK_HORIZONTAL];

	void createBlock(int x, int y, int type);
	int getBlockData(int x, int y);
	void setBlockData(int x, int y, int type);
	Sprite* getBlockSprite(int x, int y);
	void setBlockSprite(int x, int y, Sprite* s);
	void destroyBlcok(int x, int y);

	Vec2 ConvertGameCoordToBlockCoord(Vec2 Gamecoord);
	Vec2 ConvertBlockCoordToGameCoord(Vec2 Blockcoord);

	// �Ʒ��������� ã�� �ö󰡸鼭 ����ִ� ����� ã��
	//-1�� ������ �Ǹ� ����ִ� ����� ���ٴ� ��

	int findEmptyBlockIndex(int x, int y);

	//y��ġ���� ã�� �ö󰡸鼭 ������� ���� ����� ã��
	//-1�� ������ �Ǹ� ������� ���� ����� ���ٴ� ���̴�.
	int findFilledBlockIndex(int x, int y);
	//��� ����߸��� �Լ�
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