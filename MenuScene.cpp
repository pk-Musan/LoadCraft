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
		if ( KeyBoard::key[KEY_INPUT_RETURN] == 1 ) {
			description = false;
			page = 0;
		}
	} else {
		if ( KeyBoard::key[KEY_INPUT_UP] == 1 ) {
			cursorPos -= 1;
			if ( cursorPos < 0 ) cursorPos = 1; // 最大値を代入
		}
		if ( KeyBoard::key[KEY_INPUT_DOWN] == 1 ) cursorPos = ( cursorPos + 1 ) % 2;

		if ( KeyBoard::key[KEY_INPUT_RETURN] == 1 ) {
			if ( cursorPos == 1 ) description = true;
			else if ( cursorPos == 0 ) {
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
			DrawStringToHandle( 50, 50, "ブロックを壊して，手に入れたブロックで道を作ろう！", GetColor( 255, 255, 255 ), Font::fonts[1] );
			DrawStringToHandle( 50, 80, "迷子のポッキョくんと一緒に出口を目指すんだ！", GetColor( 255, 255, 255 ), Font::fonts[1] );

			DrawGraph( 56, 130, Loader::imageHandles[Loader::RIGHT], TRUE );
			DrawStringToHandle( 96, 136, "ポッキョくん：探検が大好き，すぐ迷子になる", GetColor( 255, 255, 255 ), Font::fonts[1] );

			DrawGraph( 56, 190, Loader::imageHandles[Loader::BLOCK_1_CRACKED], TRUE );
			DrawStringToHandle( 96, 194, "ブロック1：一撃でこわれてしまうほどもろい", GetColor( 255, 255, 255 ), Font::fonts[1] );

			DrawGraph( 56, 240, Loader::imageHandles[Loader::BLOCK_2_CRACKED], TRUE );
			DrawStringToHandle( 96, 244, "ブロック2：ちょっと頑丈、なかなかこわれない", GetColor( 255, 255, 255 ), Font::fonts[1] );

			DrawGraph( 56, 290, Loader::imageHandles[Loader::SPRING_BLOCK], TRUE );
			DrawStringToHandle( 96, 294, "ブロック3：弾力がすさまじい，とべるかも？", GetColor( 255, 255, 255 ), Font::fonts[1] );

			DrawGraph( 40, 340, Loader::imageHandles[Loader::BLOCK_1], TRUE );
			DrawGraph( 72, 340, Loader::imageHandles[Loader::BLOCK_2], TRUE );
			DrawGraph( 40, 372, Loader::imageHandles[Loader::DISAPPEARING_BLOCK], TRUE );
			DrawGraph( 72, 372, Loader::imageHandles[Loader::UNBREAKABLE_BLOCK], TRUE );
			DrawStringToHandle( 114, 360, "壊せないブロック：その名のとおりである", GetColor( 255, 255, 255 ), Font::fonts[1] );

			DrawStringToHandle( 290, 415, "1/2", GetColor( 255, 255, 255 ), Font::fonts[1] );
			DrawStringToHandle( 450, 415, "Enter：閉じる", GetColor( 255, 255, 255 ), Font::fonts[1] );

			DrawTriangleAA( 640 - 45, 480 / 2 - 15, 640 - 45, 480 / 2 + 15, 640 - 20, 480 / 2, GetColor( 255, 255, 255 ), TRUE );
		} else if ( page == 1 ) {
			DrawStringToHandle( 50, 45, "操作説明", GetColor( 255, 255, 255 ), Font::fonts[1] );

			DrawStringToHandle( 60, 80, "← or →：移動", GetColor( 255, 255, 255 ), Font::fonts[1] );
			DrawStringToHandle( 60, 110, "Space：ジャンプ", GetColor( 255, 255, 255 ), Font::fonts[1] );
			DrawStringToHandle( 60, 140, "C +（矢印キー）：パンチ（8方向）", GetColor( 255, 255, 255 ), Font::fonts[1] );
			DrawStringToHandle( 60, 170, "Z +（矢印キー）：ブロックを置く（8方向）", GetColor( 255, 255, 255 ), Font::fonts[1] );
			DrawStringToHandle( 60, 200, "S or D：置きたいブロックの選択", GetColor( 255, 255, 255 ), Font::fonts[1] );
			DrawStringToHandle( 60, 230, "Esc：ポーズ画面（やり直し，遊び方説明）", GetColor( 255, 255, 255 ), Font::fonts[1] );

			DrawStringToHandle( 55, 270, "ブロックへの攻撃とブロックの配置は左shiftを", GetColor( 255, 255, 255 ), Font::fonts[1] );
			DrawStringToHandle( 55, 300, "一緒に押しておくとその場で止まったまま行うよ", GetColor( 255, 255, 255 ), Font::fonts[1] );

			DrawStringToHandle( 55, 340, "※ブロック3以外のブロックは，手に入れてから", GetColor( 255, 255, 255 ), Font::fonts[1] );
			DrawStringToHandle( 55, 370, "　置いてしまうとそれ以降こわせなくなるよ！", GetColor( 255, 255, 255 ), Font::fonts[1] );

			DrawStringToHandle( 290, 415, "2/2", GetColor( 255, 255, 255 ), Font::fonts[1] );
			DrawStringToHandle( 450, 415, "Enter：閉じる", GetColor( 255, 255, 255 ), Font::fonts[1] );

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
		DrawGraph( standardX, standardY, Loader::imageHandles[Loader::LEFT_ATTACK_1 + animationCount / 12], TRUE );
		DrawGraph( standardX + 32, standardY - 32, Loader::imageHandles[Loader::LEFT_ATTACK_1 + animationCount / 12], TRUE );
		DrawGraph( standardX + 32, standardY, Loader::imageHandles[Loader::LEFT_ATTACK_1 + animationCount / 12], TRUE );
		DrawGraph( standardX + 32, standardY + 32, Loader::imageHandles[Loader::LEFT_ATTACK_1 + animationCount / 12], TRUE );
		DrawGraph( standardX + 64, standardY, Loader::imageHandles[Loader::LEFT_ATTACK_1 + animationCount / 12], TRUE );
		DrawGraph( standardX + 96, standardY, Loader::imageHandles[Loader::LEFT_ATTACK_1 + animationCount / 12], TRUE );
		
		DrawStringToHandle( 75 + 75, 200, "遊び方", GetColor( 255, 255, 255 ), Font::fonts[2] );

		DrawStringToHandle( 75, 100, "ステージセレクト", GetColor( 255, 255, 255 ), Font::fonts[2] );

		DrawStringToHandle( 165, 360, "Press Enter Key", GetColor( 255, 255, 255 ), Font::fonts[2] );

		animationCount = ( animationCount + 1 ) % 24;
	}
}