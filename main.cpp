#include "DxLib.h"
#include "SceneManager.h"
#include "KeyBoard.h"

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow ) {
	int frameStartTime;

	ChangeWindowMode( TRUE );

	// �E�C���h�E�̃T�C�Y���蓮�ł͂ł����A���E�C���h�E�̃T�C�Y�ɍ��킹�Ċg������Ȃ��悤�ɂ���
	//SetWindowSizeChangeEnableFlag( FALSE, FALSE );

	// ��ʃT�C�Y�͍ő�� 640x640 �ɂ��Ă���
	SetGraphMode( 640, 480, 32 );

	//SetWindowSize( 640, 640 );

	if ( DxLib_Init() == -1 ) return -1;

	SetDrawScreen( DX_SCREEN_BACK );
	SetWaitVSyncFlag( FALSE );

	SceneManager sceneMgr;
	sceneMgr.init();

	frameStartTime = GetNowCount();

	while ( ScreenFlip() == 0 && ProcessMessage() == 0 && ClearDrawScreen() == 0 && KeyBoard::updateKey() == 0 ) { // ��ʍX�V & ���b�Z�[�W���� & ��ʏ���
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