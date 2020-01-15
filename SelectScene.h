#pragma once
#include "Scene.h"

class SelectScene : public Scene {

public:
	enum StageData {
		STAGE_1,
		STAGE_2,
		STAGE_3,

		MAX_STAGE_NUM
	};
private:
	int cursorPos;
	//int font[2];
	int animationCount;
	const char* stages[StageData::MAX_STAGE_NUM];

public:
	SelectScene();
	~SelectScene();
	void init();
	Scene* update();
	void draw();
};