#include "DxLib.h"
#include "MenuScene.h"

MenuScene::MenuScene() {

}

void MenuScene::init() {

}

Scene* MenuScene::update() {
	if ( CheckHitKeyAll() == 0 ) {
		return NULL;
	} else return this;
}

void MenuScene::draw() {

}