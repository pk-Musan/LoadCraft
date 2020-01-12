#include "DxLib.h"
#include "TitleScene.h"
#include "GameScene.h"

TitleScene::TitleScene() {

}

void TitleScene::init() {

}

Scene* TitleScene::update() {
	if ( CheckHitKey( KEY_INPUT_A ) != 0 ) {
		const char* filename = "stageData2.txt";
		GameScene* gScene = new GameScene( filename );
		return gScene;
	} else return this;
}

void TitleScene::draw() {

}