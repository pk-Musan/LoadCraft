#pragma once
#include "Scene.h"

class SelectScene : public Scene {

public:
	enum StageData {
		STAGE_1,
		STAGE_2,
		STAGE_3,
		STAGE_4,
		STAGE_5,
		STAGE_6,
		STAGE_7,
		STAGE_8,
		STAGE_9,

		MAX_STAGE_NUM
	};
private:
	int selectNum;
	int cursorPos;
	int animationCount;
	const char* stages[StageData::MAX_STAGE_NUM];
	const char* stageNames[StageData::MAX_STAGE_NUM];

public:
	SelectScene();
	~SelectScene();
	void init();
	Scene* update();
	void draw();
};