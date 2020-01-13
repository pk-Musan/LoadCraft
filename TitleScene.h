#pragma once

#include "Scene.h"

class TitleScene : public Scene {

private:
	int animationCount;

public:
	TitleScene();
	void init();
	Scene* update();
	void draw();
};