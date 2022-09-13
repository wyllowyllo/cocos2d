#ifndef SCENE_HOME

#define SCENE_HOME
#include "stdafx.h"

class SceneHome :public Scene {
public:
	Button* btnStart;
	Button* btnQuit;
	static SceneHome* create();
	virtual bool init() override;
};

#endif
