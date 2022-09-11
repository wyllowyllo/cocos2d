#include "stdafx.h"
#include "SceneIngame.h"



void SceneInGame::createBlock(int x, int y, int type)
{
	auto cache = Director::getInstance()->getTextureCache();
	auto spr = Sprite::createWithTexture(cache->getTextureForKey("res/match3_tiles_px.png"), Rect(40*type,0,40,40));
	spr->setScale(2);
	addChild(spr);
	setBlockData(x, y, type);
	setBlockSprite(x, y, spr);
}

int SceneInGame::getBlockData(int x, int y)
{
	return BlockData[y][x];
}

void SceneInGame::setBlockData(int x, int y, int type)
{
	BlockData[y][x] = type;
}

Sprite* SceneInGame::getBlockSprite(int x, int y)
{
	return BlockSprite[y][x];
}

void SceneInGame::setBlockSprite(int x, int y, Sprite* s)
{
	BlockSprite[y][x] = s;
}

void SceneInGame::destroyBlcok(int x, int y)
{
	if (BlockData[y][x] != 0) {
		//BlockSprite[y][x]->removeFromParent();
		BlockSprite[y][x]->runAction(Sequence::create(
			FadeOut::create(0.125f),
			FadeIn::create(0.125f),
			FadeOut::create(0.125f),
			FadeIn::create(0.125f),
			Spawn::create(ScaleTo::create(0.125f, 0.0), FadeOut::create(0.125f), nullptr),
			RemoveSelf::create(),
			nullptr
		));
		BlockSprite[y][x] = nullptr;
		BlockData[y][x] = 0;
	}
}

Vec2 SceneInGame::ConvertGameCoordToBlockCoord(Vec2 Gamecoord)
{
	Vec2 BlockOrigin = BLOCK_OFFSET -
		Vec2((BLOCK_HORIZONTAL * BLOCK_WIDTH) / 2, (BLOCK_VERTICAL * BLOCK_HEIGHT) / 2)
		+ Vec2(BLOCK_WIDTH, BLOCK_HEIGHT) / 2;
	Vec2 delta = Gamecoord - BlockOrigin;
	Vec2 pos = Vec2((int)(delta.x /BLOCK_WIDTH + 0.5), (int)(delta.y /BLOCK_HEIGHT + 0.5));

	return pos;
}

Vec2 SceneInGame::ConvertBlockCoordToGameCoord(Vec2 Blockcoord)
{
	Vec2 blockOrigin = BLOCK_OFFSET - Vec2((BLOCK_HORIZONTAL * BLOCK_WIDTH) / 2, (BLOCK_VERTICAL * BLOCK_HEIGHT) / 2)
		+ Vec2(BLOCK_WIDTH, BLOCK_HEIGHT) / 2;
	return blockOrigin + Vec2(BLOCK_WIDTH * Blockcoord.x, BLOCK_HEIGHT * Blockcoord.y);
}

int SceneInGame::findEmptyBlockIndex(int x, int y)
{
	for (int i = y; i < BLOCK_VERTICAL; i++) {
		if (getBlockData(x, i) == 0) return i;
	}
	return -1;
}

int SceneInGame::findFilledBlockIndex(int x, int y)
{
	for (int i = y; i < BLOCK_VERTICAL; i++) {
		if (getBlockData(x, i) != 0) return i;
	}
	return -1;
}

void SceneInGame::DropBlock(int x)
{
	
	for (int i = 0; i < BLOCK_VERTICAL; i++) {
		int empty_y = findEmptyBlockIndex(x, i);
		if (empty_y == -1) continue;
		int filled_y = findFilledBlockIndex(x, empty_y +1);
		if (filled_y == -1) continue;

		{
			int a = getBlockData(x, empty_y);
			int b = getBlockData(x, filled_y);
			SWAP(int, a, b);
			setBlockData(x, empty_y, a);
			setBlockData(x, filled_y, b);
		} {

			Sprite* a = getBlockSprite(x, empty_y);
			Sprite* b = getBlockSprite(x, filled_y);

			SWAP(Sprite*, a, b);
			setBlockSprite(x, empty_y, a);
			setBlockSprite(x, filled_y, b);

			a->runAction(Sequence::create(
				DelayTime::create(0.625f),
				MoveTo::create(0.125f, ConvertBlockCoordToGameCoord(Vec2(x,empty_y))),
				nullptr
			));
		}
		
	}
	//alignBlcokSprite(); ->Animation effect(Dropping block below)  added 
	for (int i = 0; i < BLOCK_VERTICAL; i++)
		judgeMatch(x, i);
}

void SceneInGame::stackPush(Vec2 value)
{
	if (judgeData[(int)value.y][(int)value.x] != 0) return; //prevent from overlap insert
	judgeStack[judgeStackCount++] = value;
	judgeData[(int)value.y][(int)value.x] = 1;
}

Vec2 SceneInGame::stackPop()
{
	auto ret = judgeStack[--judgeStackCount];
	judgeData[(int)ret.y][(int)ret.x] = 0;
	return ret;
}

void SceneInGame::stackEmpty()
{
	judgeStackCount = 0;
	for (int i = 0; i < BLOCK_HORIZONTAL; i++) {
		for (int k = 0; k < BLOCK_VERTICAL; k++)
			judgeData[k][i] = 0;
	}
}

bool SceneInGame::stackFind(Vec2 value)
{
	return judgeData[(int)value.y][(int)value.x] == 1;
}

void SceneInGame::judgeMatch(int x, int y)
{
	int curblock = getBlockData(x, y);
	if (curblock == 0) return;
	int push_cnt = 0;
	stackPush(Vec2(x, y));
	for (int i = 0; i < 4; i++) {
		int inc_x;
		int inc_y;
		int cur_x = x;
		int cur_y = y;
		

		switch (i) {
		case 0: inc_x = 1; inc_y = 0; push_cnt = 0; break;
		case 1: inc_x = -1; inc_y = 0;  break;
		case 2: inc_x = 0; inc_y = 1; push_cnt=0;break;
		case 3: inc_x = 0; inc_y = -1;  break;
		}
		while (true) {
			cur_x += inc_x;
			cur_y += inc_y;
			if (cur_x > BLOCK_HORIZONTAL || cur_x < 0) break;
			if (cur_y > BLOCK_VERTICAL || cur_y < 0) break;

			if (getBlockData(cur_x, cur_y) == curblock) {
				stackPush(Vec2(cur_x, cur_y));
				push_cnt++;
			}
			else break;
		}
		if (i % 2 == 0) continue;

		if (push_cnt < 2)
		{
			for (int i = 0; i < push_cnt; i++)
				stackPop();
		}

		if (judgeStackCount >1) {
			while (judgeStackCount > 0) {
				Vec2 a=stackPop();
				destroyBlcok(a.x, a.y);
			}

		}
		stackEmpty();
	}

}

SceneInGame* SceneInGame::create()
{
	auto ret = new SceneInGame();
	if (ret && ret->init()) ret->autorelease();
	else CC_SAFE_DELETE(ret);
	return ret;
}

bool SceneInGame::init()
{
	if (!Scene::init()) return false;

	srand(time(0));
	Director::getInstance()->getTextureCache()->addImage("res/match3_tiles_px.png");

	auto touch = EventListenerTouchOneByOne::create();
	
	touch->onTouchBegan = std::bind(&SceneInGame::onTouchBegan, this, std::placeholders::_1, std::placeholders::_2);
	touch->onTouchMoved = std::bind(&SceneInGame::onTouchMoved, this, std::placeholders::_1, std::placeholders::_2);
	touch->onTouchEnded = std::bind(&SceneInGame::onTouchEnded, this, std::placeholders::_1, std::placeholders::_2);
	touch->onTouchCancelled = touch->onTouchEnded;

	getEventDispatcher()->addEventListenerWithSceneGraphPriority(touch, this);
 return true;
}

void SceneInGame::onEnter()
{
	Scene::onEnter();

	this->initUI();
	this->initGame();
	this->StartGame();
}

void SceneInGame::initUI()
{

}

void SceneInGame::initGame()
{
	for (int i = 0; i < BLOCK_HORIZONTAL; i++)
	{
		for (int k = 0; k < BLOCK_VERTICAL; k++)
			createBlock(i,k,rand()%4+1);
	}
	this->alignBlcokSprite();
}

void SceneInGame::destroyUI()
{
}

void SceneInGame::destroyGame()
{
}

void SceneInGame::alignBlcokSprite()
{
	for (int i = 0; i < BLOCK_HORIZONTAL; i++) {
		for (int k = 0; k < BLOCK_VERTICAL; k++) {
			auto s = getBlockSprite(i, k);
			if (s != nullptr) s->setPosition(ConvertBlockCoordToGameCoord(Vec2(i, k)));
		}
	}
}

bool SceneInGame::onTouchBegan(Touch* t, Event* e)
{
	Vec2 p = ConvertGameCoordToBlockCoord(t->getLocation());

	CCLOG("%f, %f", p.x, p.y);
	destroyBlcok(p.x, p.y);
	DropBlock(p.x);
	return true;
}

void SceneInGame::onTouchMoved(Touch* t, Event* e)
{
}

void SceneInGame::onTouchEnded(Touch* t, Event* e)
{
}

void SceneInGame::StartGame()
{
}

void SceneInGame::PauseGame()
{
}

void SceneInGame::WinGame()
{
}

void SceneInGame::LoseGame()
{
}
