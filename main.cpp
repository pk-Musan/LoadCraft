#include "DxLib.h"
#include "Player.h"
#include "Map.h"

#include <iostream>
#include <fstream>
#include <stdio.h>

void init( Player *pl, Map *map );
void input();
void gameloop();

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow ) {
	if ( DxLib_Init() == -1 ) {
		return -1;
	}
	/*
	DrawPixel( 320, 240, GetColor( 255, 255, 255 ) );

	WaitKey();
	*/

	gameloop();

	DxLib_End();

	return 0;
}

void init( Player *pl, Map *map ) {
	SetDrawScreen( DX_SCREEN_BACK );
	SetWaitVSyncFlag( FALSE );
	
	char filename[] = "stage.txt";
	pl = new Player();
	map = new Map( filename, pl );
}

void input() {
	/*
	キー入力に応じてplayerのメソッドを呼び出す
	int key_input = キー入力の関数
	float px = player->getX();
	float py = player->getY();
	float psize = player->getSize();
	
	switch ( key_input ) {
		case '' : // w, space同時押しで上方向のブロックを壊すとか
			if ( player->breakBlock( map ) ) {
				map->update( px / psize, py / psize - 1, 0 );
			}
			break;

		case '' : // w, s同時押しで上方向にブロックをセット
			if ( player->setBlock() ) {
				map->update
			}
	}
	*/
}

void gameloop() {
	Player *player = 0;
	Map *map = 0;

	init( player , map );

	while ( ProcessMessage() == 0 && CheckHitKey( KEY_INPUT_ESCAPE ) == 0 ) {
		ClearDrawScreen();

		input();

		// chara.move();

		// map.draw();

		// chara.draw();

		ScreenFlip();
	}

}