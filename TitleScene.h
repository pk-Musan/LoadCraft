#pragma once

#include "Scene.h"

class TitleScene : public Scene {

private:
	int imageHandle;

public:
	TitleScene();
	void init();
	Scene* update();
	void draw();
};