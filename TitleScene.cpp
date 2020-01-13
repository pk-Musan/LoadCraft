#include "DxLib.h"
#include "TitleScene.h"
#include "GameScene.h"
#include "MenuScene.h"
#include "Loader.h"

TitleScene::TitleScene() : animationCount(0) {

}

void TitleScene::init() {

}

Scene* TitleScene::update() {
	if ( CheckHitKey( KEY_INPUT_A ) != 0 ) {
	//if ( CheckHitKeyAll() != 0 ) {
		MenuScene* mScene = new MenuScene();
		return mScene;
	} else return this;
}

void TitleScene::draw() {
	DrawGraph( 120, 100, Loader::imageHandles[Loader::TITLE], TRUE );
	DrawGraph( 165, 350, Loader::imageHandles[Loader::PRESS_1 + animationCount / 30], TRUE );
	DrawGraph( 305, 350, Loader::imageHandles[Loader::ANY_KEY_1 + animationCount / 30], TRUE );

	animationCount = ( animationCount + 1 ) % 60;
}