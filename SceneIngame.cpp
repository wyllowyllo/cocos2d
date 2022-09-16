#include "stdafx.h"
#include "SceneIngame.h"
#include "SceneHome.h"



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

void SceneInGame::destroyBlock(int x, int y)
{
	if (BlockData[y][x] != 0) {
		state = GameState::BLOCK_MOVING;
		//BlockSprite[y][x]->removeFromParent();
		BlockSprite[y][x]->runAction(Sequence::create(
			FadeOut::create(0.125f),
			FadeIn::create(0.125f),
			FadeOut::create(0.125f),
			FadeIn::create(0.125f),
			Spawn::create(ScaleTo::create(0.125f, 0.0), FadeOut::create(0.0f), nullptr),
			RemoveSelf::create(),
			nullptr
		));

		BlockSprite[y][x] = nullptr;
		BlockData[y][x] = 0;
		//Global::getInstance()->blockPopMusic();

		this->runAction(Sequence::create(
			DelayTime::create(0.625f),
			CallFunc::create([=]() {DropBlock(x); }),
			nullptr
		));
	}
}

Vec2 SceneInGame::ConvertGameCoordToBlockCoord(const Vec2& Gamecoord)
{
	Vec2 BlockOrigin = BLOCK_OFFSET -
		Vec2((BLOCK_HORIZONTAL * BLOCK_WIDTH) / 2, (BLOCK_VERTICAL * BLOCK_HEIGHT) / 2)
		+ Vec2(BLOCK_WIDTH, BLOCK_HEIGHT) / 2;
	Vec2 delta = Gamecoord - BlockOrigin;
	Vec2 pos = Vec2((int)(delta.x /BLOCK_WIDTH + 0.5), (int)(delta.y /BLOCK_HEIGHT + 0.5));

	return pos;
}

Vec2 SceneInGame::ConvertBlcokCoordToGameCoord(const Vec2& Blockcoord)
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
	bool isDrop = false; // 최상단 블록이 추가되지 않을 경우에 유효함
	for (int i = 0; i < BLOCK_VERTICAL; i++) {
		int empty_y = findEmptyBlockIndex(x, i);
		if (empty_y == -1) continue;
		int filled_y = findFilledBlockIndex(x, empty_y +1);
		if (filled_y == -1) { //restock blocks 
				createBlock(x, empty_y, rand() % BLOCK_VAR + 1);
				BlockSprite[empty_y][x]->setPosition(ConvertBlcokCoordToGameCoord(Vec2(x, BLOCK_VERTICAL+1)));
				BlockSprite[empty_y][x]->runAction(MoveTo::create(0.125f, ConvertBlcokCoordToGameCoord(Vec2(x, empty_y))));
				isDrop = true;
				continue;
		}
			
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

			a->stopAllActions();
			a->runAction(MoveTo::create(0.125f, ConvertBlcokCoordToGameCoord(Vec2(x, empty_y))));
		}
		isDrop = true;
	}
	//alignBlcokSprite();
	if (isDrop) {
		for (int i = 0; i < BLOCK_VERTICAL; i++) {
			this->runAction(Sequence::create(DelayTime::create(0.1f), CallFunc::create([=]() {judgeMatch(x, i); }), nullptr));
			
		}
	}
	else
		state = GameState::PLAYING;
}

void SceneInGame::stackPush(const Vec2& value)
{
	if (judgeData[(int)value.y][(int)value.x] != 0) return; //prevent from overlap insert
	judgeStack[judgeStackCount++] = value;
	judgeData[(int)value.y][(int)value.x] = 1;
}

const Vec2& SceneInGame::stackPop()
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

bool SceneInGame::stackFind(const Vec2& value)
{
	return judgeData[(int)value.y][(int)value.x] == 1;
}

void SceneInGame::judgeMatch(int x, int y)
{
	int blockdata = getBlockData(x, y);
	if (blockdata == 0) return;

	stackPush(Vec2(x, y));
	int push_cnt = 0;

	for (int i = 0; i < 4; i++) {
		int next_x = x;
		int next_y = y;
		int inc_x;
		int inc_y;

		switch (i) {
		case 0: inc_x = 1; inc_y = 0; push_cnt = 0; break;
		case 1: inc_x = -1; inc_y = 0; break;
		case 2: inc_x = 0; inc_y = 1; push_cnt = 0; stackPush(Vec2(x, y)); break;
		case 3: inc_x = 0; inc_y = -1; break;
		}

		while (true) {
			next_x += inc_x;
			next_y += inc_y;
			if (next_x<0 || next_x>=BLOCK_HORIZONTAL) break;
			if (next_y<0 || next_y>=BLOCK_VERTICAL) break;

			if (getBlockData(next_x, next_y) == blockdata) {
				stackPush(Vec2(next_x, next_y));
				push_cnt++;
			}
			else break;
		}

		if (i % 2 == 0) continue;
		if (push_cnt < 2) {
			for (int i = 0; i < push_cnt; i++) {
				stackPop();
			}
		}
		if (judgeStackCount > 2) {
			Global::getInstance()->addScore(judgeStackCount * 10);
			ui->setScore(Global::getInstance()->getScore());
			while (judgeStackCount >0) {
				Vec2 p = stackPop();
				destroyBlock(p.x, p.y);
			}
			Global::getInstance()->blockPopMusic();
		}
		else
			state = GameState::PLAYING;
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
	addChild(ui = LayerIngameUI::create());
	ui->setLocalZOrder(1); // it makes UIpanel upper on gameblock

	ui->btnPause->addClickEventListener([=](Ref* r)->void {
		if (state == GameState::PLAYING) {
			ui->showPausePanel();
			state = GameState::PAUSED;
			Global::getInstance()->stopBackgroundMusic();
			Global::getInstance()->playPop();
		}
		});

	ui->btnResume->addClickEventListener([=](Ref* r)->void {
		if (state == GameState::PAUSED) {
			ui->hidePausePanel();
			state = GameState::PLAYING;
			Global::getInstance()->playPop();
			Global::getInstance()->playBackgroundMusic();
		}
		});

	ui->btnRestart->addClickEventListener([=](Ref* r)->void {
		if (state == GameState::PAUSED) {
			ui->hidePausePanel();
			ui->setScore(0);

			Global::getInstance()->playPop();
			this->destroyGame();
			this->initGame();
			this->StartGame();
			state = GameState::PLAYING;
		}

		});

	ui->btnHome->addClickEventListener([=](Ref* r)->void {
		if (state == GameState::PAUSED) {
			auto scene = SceneHome::create();
			auto transit = TransitionSlideInL::create(0.125f, scene);
			Director::getInstance()->replaceScene(transit);
			Global::getInstance()->playPop();
		}
		});

}

void SceneInGame::initGame()
{
	Global::getInstance()->setScore(0);
	for (int i = 0; i < BLOCK_HORIZONTAL; i++)
	{
		for (int k = 0; k < BLOCK_VERTICAL; k++)
			createBlock(i,k,rand()%BLOCK_VAR+1);
	}
	this->alignBlcokSprite();
}

void SceneInGame::destroyUI()
{
}

void SceneInGame::destroyGame()
{
	Global::getInstance()->setScore(0);
	for (int i = 0; i < BLOCK_HORIZONTAL; i++) {
		for (int k = 0; k < BLOCK_VERTICAL; k++) {
			setBlockData(i, k, 0);
			getBlockSprite(i,k)->removeFromParent();
			setBlockSprite(i, k, nullptr);
		}
	}
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

bool SceneInGame::onTouchBegan(Touch* t, Event* e)
{
	Vec2 p = ConvertGameCoordToBlockCoord(t->getLocation());
	Vec2 q = t->getLocation();
	CCLOG("%f, %f", q.x, q.y);
	//CCLOG("%f, %f", p.x, p.y);

	if (state == GameState::PLAYING) {  // Func 'DestroyBlock' doesn't work when the state isn't a 'PLAYING'
		if (p.x >= BLOCK_HORIZONTAL || p.x < 0) return true;
		if (p.y >= BLOCK_VERTICAL || p.y < 0) return true;

		destroyBlock(p.x, p.y);
	}
	//DropBlock(p.x);

	/*for (int i = 0; i < BLOCK_VERTICAL; i++) {
		judgeMatch(p.x, p.y);
	}*/
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
	Global::getInstance()->playBackgroundMusic();
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
