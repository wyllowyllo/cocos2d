#pragma once
#ifndef LAYER_INGAME_UI_
#define LAYER_INGAME_UI_

#include "stdafx.h"
#include "Environment.h"

class LayerIngameUI :public Node {
private:
	long long score;
	char scoreString[128];
public:
	Label* lbScore; // handle score
	Button* btnPause;
	Scale9Sprite* pausePanel;
	Button* btnResume; //Cancel the pause status
	Button* btnRestart; //restart
	Button* btnHome; //back to homepanel
	DrawNode* dnCurtain; // related to draw something

	static LayerIngameUI* create();
	virtual bool init() override;

	void setScore(long long score);
	long long getScore();
	void showPausePanel(); // load PasuePanel
	void hidePausePanel(); // Disappear PausePanel
};

#endif

