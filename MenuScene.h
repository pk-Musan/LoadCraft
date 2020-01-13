#pragma once
#include "Scene.h"

class MenuScene : public Scene {

private:
	bool description;
	int cursorPos;
	int font[2];
	int animationCount;

public:
	MenuScene();
	void init();
	Scene* update();
	void draw();
};