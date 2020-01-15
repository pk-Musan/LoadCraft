#include "DxLib.h"
#include "MenuScene.h"
#include "SelectScene.h"
#include "KeyBoard.h"
#include "Loader.h"
#include "Font.h"

MenuScene::MenuScene() {
	description = false;
	page = 0;
	cursorPos = 0;
	animationCount = 0;
}

void MenuScene::init() {

}

Scene* MenuScene::update() {
	if ( description ) {
		if ( KeyBoard::key[KEY_INPUT_LEFT] == 1 ) {
			page -= 1;
			if ( page < 0 ) page = 0;
		}
		if ( KeyBoard::key[KEY_INPUT_RIGHT] == 1 ) {
			page += 1;
			if ( page > 1 ) page = 1;
		}
		if ( KeyBoard::key[KEY_INPUT_ESCAPE] == 1 ) {
			description = false;
			page = 0;
		}
	} else {
		if ( KeyBoard::key[KEY_INPUT_UP] == 1 ) {
			cursorPos -= 1;
			if ( cursorPos < 0 ) cursorPos = 1; // 最大値を代入
		}
		if ( KeyBoard::key[KEY_INPUT_DOWN] == 1 ) cursorPos = ( cursorPos + 1 ) % 2;

		if ( KeyBoard::key[KEY_INPUT_SPACE] == 1 ) {
			if ( cursorPos == 0 ) description = true;
			else if ( cursorPos == 1 ) {
				SelectScene* sScene = new SelectScene();
				return sScene;
			}
		}
	}
	return this;
}

void MenuScene::draw() {
	for ( int x = 0; x < 20; x++ ) DrawGraph( x * 32, 0, Loader::imageHandles[Loader::BLOCK_1], TRUE );
	for ( int y = 1; y < 14; y++ ) {
		DrawGraph( 0, y * 32, Loader::imageHandles[Loader::BLOCK_1], TRUE );
		DrawGraph( 640 - 32, y * 32, Loader::imageHandles[Loader::BLOCK_1], TRUE );
	}
	for ( int x = 0; x < 20; x++ ) DrawGraph( x * 32, 480 - 32, Loader::imageHandles[Loader::BLOCK_1], TRUE );

	if ( description ) {
		// 説明を描画
		if ( page == 0 ) {
			DrawStringToHandle( 50, 50, "ブロックを壊して手に入れよう！", GetColor( 255, 255, 255 ), Font::fonts[1] );
			DrawStringToHandle( 50, 80, "手に入れたブロックで道を作ってゴールを目指そう！", GetColor( 255, 255, 255 ), Font::fonts[1] );

			DrawGraph( 56, 130, Loader::imageHandles[Loader::RIGHT], TRUE );
			DrawStringToHandle( 96, 136, "ポッキョくん：", GetColor( 255, 255, 255 ), Font::fonts[1] );
			DrawStringToHandle( 150, 166, "洞窟探検が大好き、すぐ迷子になる", GetColor( 255, 255, 255 ), Font::fonts[1] );
			DrawStringToHandle( 150, 196, "パンチでブロックをこわすことができる", GetColor( 255, 255, 255 ), Font::fonts[1] );

			DrawGraph( 56, 240, Loader::imageHandles[Loader::BLOCK_1_CRACKED], TRUE );
			DrawStringToHandle( 96, 244, "ブロック１：一撃でこわれてしまうほどもろい", GetColor( 255, 255, 255 ), Font::fonts[1] );

			DrawGraph( 56, 290, Loader::imageHandles[Loader::BLOCK_2_CRACKED], TRUE );
			DrawStringToHandle( 96, 294, "ブロック２：ちょっと頑丈、なかなかこわれない", GetColor( 255, 255, 255 ), Font::fonts[1] );

			DrawGraph( 40, 372, Loader::imageHandles[Loader::BLOCK_1], TRUE );
			DrawGraph( 72, 372, Loader::imageHandles[Loader::BLOCK_2], TRUE );
			DrawGraph( 56, 340, Loader::imageHandles[Loader::UNBREAKABLE_BLOCK], TRUE );
			DrawStringToHandle( 114, 360, "壊せないブロック：その名のとおりである", GetColor( 255, 255, 255 ), Font::fonts[1] );

			DrawStringToHandle( 290, 415, "1/2", GetColor( 255, 255, 255 ), Font::fonts[1] );
			DrawStringToHandle( 480, 415, "Esc：戻る", GetColor( 255, 255, 255 ), Font::fonts[1] );

			DrawTriangleAA( 640 - 45, 480 / 2 - 15, 640 - 45, 480 / 2 + 15, 640 - 20, 480 / 2, GetColor( 255, 255, 255 ), TRUE );
		} else if ( page == 1 ) {
			DrawStringToHandle( 50, 45, "操作説明", GetColor( 255, 255, 255 ), Font::fonts[1] );

			DrawStringToHandle( 60, 100, "← or →：移動", GetColor( 255, 255, 255 ), Font::fonts[1] );
			DrawStringToHandle( 60, 130, "Space：ジャンプ", GetColor( 255, 255, 255 ), Font::fonts[1] );
			DrawStringToHandle( 60, 160, "C +（矢印キー）：パンチ（8方向）", GetColor( 255, 255, 255 ), Font::fonts[1] );
			DrawStringToHandle( 60, 190, "Z +（矢印キー）：ブロックを置く（8方向）", GetColor( 255, 255, 255 ), Font::fonts[1] );
			DrawStringToHandle( 60, 220, "S or D：置きたいブロックの選択", GetColor( 255, 255, 255 ), Font::fonts[1] );
			DrawStringToHandle( 60, 250, "Esc：ポーズ画面", GetColor( 255, 255, 255 ), Font::fonts[1] );

			DrawStringToHandle( 55, 290, "ブロックへの攻撃とブロックの配置は左shiftを", GetColor( 255, 255, 255 ), Font::fonts[1] );
			DrawStringToHandle( 55, 320, "一緒に押しておくとその場で止まったまま行うよ", GetColor( 255, 255, 255 ), Font::fonts[1] );

			DrawStringToHandle( 55, 370, "※自分で置いたブロックはそれ以降こわせなくなるよ", GetColor( 255, 255, 255 ), Font::fonts[1] );

			DrawStringToHandle( 290, 415, "2/2", GetColor( 255, 255, 255 ), Font::fonts[1] );
			DrawStringToHandle( 480, 415, "Esc：戻る", GetColor( 255, 255, 255 ), Font::fonts[1] );

			DrawTriangleAA( 45, 480 / 2 - 15, 45, 480 / 2 + 15, 20, 480 / 2, GetColor( 255, 255, 255 ), TRUE );
		}
	} else {
		int standardX, standardY;
		if ( cursorPos == 0 ) {
			standardX = 400;
			standardY = 100 - 1;
		} else {
			standardX = 400;
			standardY = 200 - 1;
		}
		DrawGraph( standardX, standardY, Loader::imageHandles[Loader::LEFT + animationCount / 8], TRUE );
		DrawGraph( standardX + 32, standardY - 32, Loader::imageHandles[Loader::LEFT + animationCount / 8], TRUE );
		DrawGraph( standardX + 32, standardY, Loader::imageHandles[Loader::LEFT + animationCount / 8], TRUE );
		DrawGraph( standardX + 32, standardY + 32, Loader::imageHandles[Loader::LEFT + animationCount / 8], TRUE );
		DrawGraph( standardX + 64, standardY, Loader::imageHandles[Loader::LEFT + animationCount / 8], TRUE );
		DrawGraph( standardX + 96, standardY, Loader::imageHandles[Loader::LEFT + animationCount / 8], TRUE );
		
		DrawStringToHandle( 70 + 75, 100, "遊び方", GetColor( 255, 255, 255 ), Font::fonts[2] );

		DrawStringToHandle( 70, 200, "ステージセレクト", GetColor( 255, 255, 255 ), Font::fonts[2] );

		DrawStringToHandle( 160, 360, "Press Space Key", GetColor( 255, 255, 255 ), Font::fonts[2] );

		animationCount = ( animationCount + 1 ) % 24;
	}
}