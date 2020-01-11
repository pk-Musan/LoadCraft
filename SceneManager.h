#pragma once

#include "Scene.h"

class SceneManager {
private:
	Scene *mScene;

public:
	void init();
	Scene* update();
	void draw();
	void finalize();
};