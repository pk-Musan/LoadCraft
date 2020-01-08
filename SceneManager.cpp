#include "SceneManager.h"

void SceneManager::init() {
	mScene->init();
}

void SceneManager::update() {
	Scene *nextScene = mScene->update();

	if ( nextScene != mScene ) {
		delete mScene;
		mScene = 0;
		mScene = nextScene;

		mScene->init();
	}

	mScene->update();
}

void SceneManager::draw() {
	mScene->draw();
}