#include "SceneManager.h"
#include "TitleScene.h"

void SceneManager::init() {
	mScene = new TitleScene();
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