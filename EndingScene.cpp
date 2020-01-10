#include "DxLib.h"
#include "EndingScene.h"

EndingScene::EndingScene() {

}

void EndingScene::init() {

}

Scene* EndingScene::update() {
	if ( CheckHitKeyAll() == 0 ) {
		return NULL;
	} else return this;
}

void EndingScene::draw() {

}