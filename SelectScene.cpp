#include "DxLib.h"
#include "SelectScene.h"
#include "MenuScene.h"
#include "GameScene.h"
#include "KeyBoard.h"
#include "Loader.h"
#include "Font.h"

SelectScene::SelectScene() {
	/*
	font[0] = CreateFontToHandle( NULL, 20, 3, DX_FONTTYPE_ANTIALIASING_EDGE );
	font[1] = CreateFontToHandle( NULL, 36, 3, DX_FONTTYPE_ANTIALIASING_EDGE );
	*/
	cursorPos = 0;
	animationCount = 0;
	
	stages[STAGE_1] = "Stage/stageData1.txt";
	stages[STAGE_2] = "Stage/stageData2.txt";
	stages[STAGE_3] = "Stage/stageData3.txt";
}

SelectScene::~SelectScene() {
	//delete stages;
}

void SelectScene::init() {

}

Scene *SelectScene::update() {
	if ( KeyBoard::key[KEY_INPUT_UP] == 1 ) {
		cursorPos -= 1;
		if ( cursorPos < 0 ) cursorPos = MAX_STAGE_NUM - 1; // 最大値を代入
	}
	if ( KeyBoard::key[KEY_INPUT_DOWN] == 1 ) cursorPos = ( cursorPos + 1 ) % MAX_STAGE_NUM;

	if ( KeyBoard::key[KEY_INPUT_SPACE] == 1 ) {
		GameScene* gScene = new GameScene( stages[cursorPos] );
		return gScene;
	}
	if ( KeyBoard::key[KEY_INPUT_ESCAPE] == 1 ) {
		MenuScene* mScene = new MenuScene();
		return mScene;
	}

	return this;
}

void SelectScene::draw() {
	int standardX, standardY;
	standardX = 400;
	standardY = 100 + cursorPos * 60 - 1;

	for ( int x = 0; x < 20; x++ ) DrawGraph( x * 32, 0, Loader::imageHandles[Loader::BLOCK_2], TRUE );
	for ( int y = 1; y < 14; y++ ) {
		DrawGraph( 0, y * 32, Loader::imageHandles[Loader::BLOCK_2], TRUE );
		DrawGraph( 640 - 32, y * 32, Loader::imageHandles[Loader::BLOCK_2], TRUE );
	}
	for ( int x = 0; x < 20; x++ ) DrawGraph( x * 32, 480 - 32, Loader::imageHandles[Loader::BLOCK_2], TRUE );
	
	DrawGraph( standardX, standardY, Loader::imageHandles[Loader::LEFT + animationCount / 8], TRUE );
	DrawGraph( standardX + 32, standardY - 32, Loader::imageHandles[Loader::LEFT + animationCount / 8], TRUE );
	DrawGraph( standardX + 32, standardY, Loader::imageHandles[Loader::LEFT + animationCount / 8], TRUE );
	DrawGraph( standardX + 32, standardY + 32, Loader::imageHandles[Loader::LEFT + animationCount / 8], TRUE );
	DrawGraph( standardX + 64, standardY, Loader::imageHandles[Loader::LEFT + animationCount / 8], TRUE );
	DrawGraph( standardX + 96, standardY, Loader::imageHandles[Loader::LEFT + animationCount / 8], TRUE );

	DrawStringToHandle( 120, 100, "ステージ１", GetColor( 255, 255, 255 ), Font::fonts[2] );

	DrawStringToHandle( 120, 160, "ステージ２", GetColor( 255, 255, 255 ), Font::fonts[2] );

	DrawStringToHandle( 120, 220, "ステージ３", GetColor( 255, 255, 255 ), Font::fonts[2] );

	DrawStringToHandle( 160, 330, "Press Space Key", GetColor( 255, 255, 255 ), Font::fonts[2] );

	DrawStringToHandle( 380, 415, "Esc：メニューに戻る", GetColor( 255, 255, 255 ), Font::fonts[1] );

	animationCount = ( animationCount + 1 ) % 24;
}