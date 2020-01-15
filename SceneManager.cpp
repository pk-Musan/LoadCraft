#include "SceneManager.h"
#include "TitleScene.h"

#include <iostream>

void SceneManager::init() {
	mScene = new TitleScene();
	//std::cout << mScene << std::endl;
	mScene->init();
}

Scene* SceneManager::update() {
	Scene *nextScene = mScene->update();
	//std::cout << nextScene << std::endl;
	//std::cout << mScene << std::endl;
	if ( nextScene != mScene ) {
		delete mScene;
		//mScene = 0;
		mScene = nextScene;

		if ( mScene != NULL ) mScene->init();
	}

	return mScene;
	//mScene->update();
}

void SceneManager::draw() {
	mScene->draw();
}

void SceneManager::finalize() {
	delete mScene;
	mScene = 0;
}