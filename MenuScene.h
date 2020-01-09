#pragma once
#include "Scene.h"

class MenuScene : public Scene {

private:

public:
	MenuScene();
	void init();
	Scene* update();
	void draw();
};