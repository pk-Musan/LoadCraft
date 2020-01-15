#pragma once
#include "Scene.h"

class MenuScene : public Scene {

private:
	bool description;
	int page;
	int cursorPos;
	int animationCount;

public:
	MenuScene();
	void init();
	Scene* update();
	void draw();
};