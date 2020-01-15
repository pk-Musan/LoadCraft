#include "DxLib.h"
#include "SceneManager.h"
#include "KeyBoard.h"
#include "Loader.h"
#include "Font.h"

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow ) {
	int frameStartTime;

	ChangeWindowMode( TRUE );

	// ウインドウのサイズを手動ではできず、且つウインドウのサイズに合わせて拡大もしないようにする
	//SetWindowSizeChangeEnableFlag( FALSE, FALSE );

	// 画面サイズは最大の 640x640 にしておく
	SetGraphMode( 640, 480, 32 );

	//SetWindowSize( 640, 640 );

	if ( DxLib_Init() == -1 ) return -1;

	SetDrawScreen( DX_SCREEN_BACK );
	SetWaitVSyncFlag( FALSE );

	Loader::loadGraph();
	Font::createFont();

	SceneManager sceneMgr;
	sceneMgr.init();

	frameStartTime = GetNowCount();

	while ( ScreenFlip() == 0 && ProcessMessage() == 0 && ClearDrawScreen() == 0 && KeyBoard::updateKey() == 0 ) { // 画面更新 & メッセージ処理 & 画面消去
		while ( GetNowCount() - frameStartTime < 1000/60 ){}

		frameStartTime = GetNowCount();

		if ( sceneMgr.update() == NULL || KeyBoard::key[KEY_INPUT_Q] == 1 ) {
			sceneMgr.finalize();
			break;
		}
		sceneMgr.draw();
	}

	Loader::deleteGraph();

	DxLib_End();
	return 0;
}