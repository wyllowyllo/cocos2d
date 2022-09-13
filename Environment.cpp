#include "stdafx.h"
#include "Environment.h"

Global* instance = nullptr;

Global* Global::getInstance()
{
	if (instance == nullptr) instance = new Global();
	return instance;
}

void Global::setScore(long long value)
{
	this->score = value;
}

long long Global::getScore()
{
	return score;
}

void Global::addScore(long long value)
{
	this->score += value;
}

void Global::playPop()
{
	AudioEngine::play2d("res/pop.mp3");
}

void Global::playBackgroundMusic()
{
	this->backgoundMusicId=AudioEngine::play2d("res/Attic Secrets.mp3", true, 0.5f);
}

void Global::stopMackgroundMusic()
{
	if (backgoundMusicId == -1) return;
	AudioEngine::stop(this->backgoundMusicId);
}

void Global::blockPopMusic()
{
	//TODO: Add a blockPop sound effect
}
