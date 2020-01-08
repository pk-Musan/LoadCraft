#include "DxLib.h"
#include "SceneManager.h"

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow ) {

	ChangeWindowMode( TRUE );
	if ( DxLib_Init() == -1 ) return -1;

	SetDrawScreen( DX_SCREEN_BACK );
	SetWaitVSyncFlag( FALSE );

	SceneManager sceneMgr;
	sceneMgr.init();

	while ( ScreenFlip() == 0 && ProcessMessage() == 0 && ClearDrawScreen() == 0 ) { // ��ʍX�V & ���b�Z�[�W���� & ��ʏ���
		sceneMgr.update();
		sceneMgr.draw();
	}

	DxLib_End();
	return 0;
}