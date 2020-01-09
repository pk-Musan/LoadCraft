#pragma once
#include "Scene.h"

class EndingScene : public Scene {

private:

public:
	EndingScene();
	void init();
	Scene* update();
	void draw();
};