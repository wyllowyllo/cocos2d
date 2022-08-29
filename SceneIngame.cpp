#include "stdafx.h"
#include "SceneIngame.h"



void SceneInGame::createBlock(int x, int y, int type)
{
	auto cache = Director::getInstance()->getTextureCache();
	auto spr = Sprite::createWithTexture(cache->getTextureForKey("res/match3_tiles_px.png"), Rect(0,0,40,40));
	spr->setScale(2);
	//spr->setPosition(Vec2(1280 / 2, 720 / 2));
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
		BlockSprite[y][x]->removeFromParent();
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
	Vec2 pos = Vec2((int)(delta.x / BLOCK_WIDTH + 0.5), (int)(delta.y / BLOCK_HEIGHT + 0.5));

	return pos;
}

Vec2 SceneInGame::ConvertBlcokCoordToGameCoord(Vec2 Blockcoord)
{
	Vec2 blockOrigin = BLOCK_OFFSET - Vec2((BLOCK_HORIZONTAL * BLOCK_WIDTH) / 2, (BLOCK_VERTICAL * BLOCK_HEIGHT) / 2)
		+ Vec2(BLOCK_WIDTH, BLOCK_HEIGHT) / 2;
	return blockOrigin + Vec2(BLOCK_WIDTH * Blockcoord.x, BLOCK_HEIGHT * Blockcoord.y);
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

	Director::getInstance()->getTextureCache()->addImage("res/match3_tiles_px.png");

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
			createBlock(i,k,1);
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
			if (s != nullptr) s->setPosition(ConvertBlcokCoordToGameCoord(Vec2(i, k)));
		}
	}
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
