#pragma once

#include "Scene.h"

class SceneManager {
private:
	Scene *mScene;

public:
	void init();
	void update();
	void draw();
};