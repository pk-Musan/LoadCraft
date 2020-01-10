#include "SceneManager.h"
#include "TitleScene.h"

#include <iostream>

void SceneManager::init() {
	mScene = new TitleScene();
	//std::cout << mScene << std::endl;
	mScene->init();
}

void SceneManager::update() {
	Scene *nextScene = mScene->update();
	//std::cout << nextScene << std::endl;
	//std::cout << mScene << std::endl;
	if ( nextScene != mScene && nextScene != NULL) {
		delete mScene;
		//mScene = 0;
		mScene = nextScene;

		mScene->init();
	}

	//mScene->update();
}

void SceneManager::draw() {
	mScene->draw();
}