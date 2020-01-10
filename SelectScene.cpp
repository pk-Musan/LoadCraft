#include "DxLib.h"
#include "SelectScene.h"

SelectScene::SelectScene() {

}

void SelectScene::init() {

}

Scene *SelectScene::update() {
	if ( CheckHitKeyAll() == 0 ) {
		return NULL;
	} else return this;
}

void SelectScene::draw() {

}