#pragma once
#include "Scene.h"

class SelectScene : public Scene {

private:

public:
	SelectScene();
	void init();
	Scene* update();
	void draw();
};