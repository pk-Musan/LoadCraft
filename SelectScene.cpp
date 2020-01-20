#include "DxLib.h"
#include "SelectScene.h"
#include "MenuScene.h"
#include "GameScene.h"
#include "KeyBoard.h"
#include "Loader.h"
#include "Font.h"

SelectScene::SelectScene() {
	selectNum = STAGE_1;
	cursorPos = 1;
	animationCount = 0;
	
	stages[STAGE_1] = "Stage/stageData1.txt";
	stages[STAGE_2] = "Stage/stageData2.txt";
	stages[STAGE_3] = "Stage/stageData3.txt";
	stages[STAGE_4] = "Stage/stageData4.txt";
	stages[STAGE_5] = "Stage/stageData5.txt";
	stages[STAGE_6] = "Stage/stageData6.txt";
	stages[STAGE_7] = "Stage/stageData7.txt";
	stages[STAGE_8] = "Stage/stageData8.txt";
	stages[STAGE_9] = "Stage/stageData9.txt";

	stageNames[STAGE_1] = "ステージ1";
	stageNames[STAGE_2] = "ステージ2";
	stageNames[STAGE_3] = "ステージ3";
	stageNames[STAGE_4] = "ステージ4";
	stageNames[STAGE_5] = "ステージ5";
	stageNames[STAGE_6] = "ステージ6";
	stageNames[STAGE_7] = "ステージ7";
	stageNames[STAGE_8] = "ステージ8";
	stageNames[STAGE_9] = "ステージ9";
}

SelectScene::~SelectScene() {
	//delete stages;
}

void SelectScene::init() {

}

Scene *SelectScene::update() {
	if ( KeyBoard::key[KEY_INPUT_LEFT] == 1 ) {
		//cursorPos -= 1;
		selectNum -= 1;
		if ( cursorPos < 0 ) cursorPos = 0; // 最大値を代入
		if ( selectNum < 0 ) selectNum = MAX_STAGE_NUM - 1;
	}
	if ( KeyBoard::key[KEY_INPUT_RIGHT] == 1 ) {
		//cursorPos += 1;
		selectNum = ( selectNum + 1 ) % MAX_STAGE_NUM;
		if ( cursorPos > 2 ) cursorPos = 2;
	}

	if ( KeyBoard::key[KEY_INPUT_SPACE] == 1 ) {
		GameScene* gScene = new GameScene( stages[selectNum] );
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
	standardX = 70 + 64;
	standardY = 100 + cursorPos * 60 - 1;

	for ( int x = 0; x < 20; x++ ) DrawGraph( x * 32, 0, Loader::imageHandles[Loader::BLOCK_2], TRUE );
	for ( int y = 1; y < 14; y++ ) {
		DrawGraph( 0, y * 32, Loader::imageHandles[Loader::BLOCK_2], TRUE );
		DrawGraph( 640 - 32, y * 32, Loader::imageHandles[Loader::BLOCK_2], TRUE );
	}
	for ( int x = 0; x < 20; x++ ) DrawGraph( x * 32, 480 - 32, Loader::imageHandles[Loader::BLOCK_2], TRUE );
	
	DrawGraph( standardX, standardY, Loader::imageHandles[Loader::LEFT_ATTACK_1 + animationCount / 12], TRUE );
	DrawGraph( standardX + 32, standardY - 32, Loader::imageHandles[Loader::LEFT_ATTACK_1 + animationCount / 12], TRUE );
	DrawGraph( standardX + 32, standardY, Loader::imageHandles[Loader::LEFT_ATTACK_1 + animationCount / 12], TRUE );
	DrawGraph( standardX + 32, standardY + 32, Loader::imageHandles[Loader::LEFT_ATTACK_1 + animationCount / 12], TRUE );
	//DrawGraph( standardX + 64, standardY, Loader::imageHandles[Loader::LEFT_ATTACK_1 + animationCount / 12], TRUE );
	//DrawGraph( standardX + 96, standardY, Loader::imageHandles[Loader::LEFT_ATTACK_1 + animationCount / 12], TRUE );

	DrawGraph( 640 - standardX - 32, standardY, Loader::imageHandles[Loader::RIGHT_ATTACK_1 + animationCount / 12], TRUE );
	DrawGraph( 640 - standardX - 32 - 32, standardY - 32, Loader::imageHandles[Loader::RIGHT_ATTACK_1 + animationCount / 12], TRUE );
	DrawGraph( 640 - standardX - 32 - 32, standardY, Loader::imageHandles[Loader::RIGHT_ATTACK_1 + animationCount / 12], TRUE );
	DrawGraph( 640 - standardX - 32 - 32, standardY + 32, Loader::imageHandles[Loader::RIGHT_ATTACK_1 + animationCount / 12], TRUE );
	//DrawGraph( 640 - standardX - 32 - 64, standardY, Loader::imageHandles[Loader::RIGHT_ATTACK_1 + animationCount / 12], TRUE );
	//DrawGraph( 640 - standardX - 32 - 96, standardY, Loader::imageHandles[Loader::RIGHT_ATTACK_1 + animationCount / 12], TRUE );

	//DrawStringToHandle( 120, 100, stageNames[cursorPos], GetColor( 255, 255, 255 ), Font::fonts[2] );

	DrawStringToHandle( 225, 160, stageNames[selectNum], GetColor( 255, 255, 255 ), Font::fonts[2] );
	/*
	if ( cursorPos + 1 < MAX_STAGE_NUM ) {
		DrawStringToHandle( 120, 160, stageNames[cursorPos + 1], GetColor( 255, 255, 255 ), Font::fonts[2] );
	}

	if ( cursorPos + 2 < MAX_STAGE_NUM ) {
		DrawStringToHandle( 120, 220, stageNames[cursorPos + 2], GetColor( 255, 255, 255 ), Font::fonts[2] );
	}
	*/
	DrawStringToHandle( 165, 330, "Press Space Key", GetColor( 255, 255, 255 ), Font::fonts[2] );

	DrawStringToHandle( 380, 415, "Esc：メニューに戻る", GetColor( 255, 255, 255 ), Font::fonts[1] );

	animationCount = ( animationCount + 1 ) % 24;
}