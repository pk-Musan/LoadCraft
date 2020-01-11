#include "DxLib.h"
#include "SceneManager.h"
#include "KeyBoard.h"

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow ) {
	int frameStartTime;

	ChangeWindowMode( TRUE );
	if ( DxLib_Init() == -1 ) return -1;

	SetDrawScreen( DX_SCREEN_BACK );
	SetWaitVSyncFlag( FALSE );

	SceneManager sceneMgr;
	sceneMgr.init();

	frameStartTime = GetNowCount();

	while ( ScreenFlip() == 0 && ProcessMessage() == 0 && ClearDrawScreen() == 0 && KeyBoard::updateKey() == 0 ) { // 画面更新 & メッセージ処理 & 画面消去
		while ( GetNowCount() - frameStartTime < 1000/60 ){}

		frameStartTime = GetNowCount();

		if ( sceneMgr.update() == NULL ) {
			sceneMgr.finalize();
			break;
		}
		sceneMgr.draw();
	}

	DxLib_End();
	return 0;
}