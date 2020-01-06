#include "DxLib.h"
#include "Game.h"
#include "Player.h"
#include "Map.h"

#include <iostream>
#include <fstream>
#include <stdio.h>

Game game;

void gameloop();
void init( Player *pl, Map *map );
void input();

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow ) {
	if ( DxLib_Init() == -1 ) {
		return -1;
	}
	/*
	DrawPixel( 320, 240, GetColor( 255, 255, 255 ) );

	WaitKey();
	*/

	game.loop();

	DxLib_End();

	return 0;
}

void gameloop() {
	Player *player = 0;
	Map *map = 0;

	init( player, map );

	while ( ProcessMessage() == 0 && CheckHitKey( KEY_INPUT_ESCAPE ) == 0 ) {
		ClearDrawScreen();

		input();

		// chara.move();

		// map.draw();

		// chara.draw();

		ScreenFlip();
	}
}

void init( Player *pl, Map *map ) {
	SetDrawScreen( DX_SCREEN_BACK );
	SetWaitVSyncFlag( FALSE );

	char filename[] = "stage.txt";

	pl = new Player();
	map = new Map( filename );
	int startX, startY;
	std::tie( startX, startY ) = map->getStartPos();
	pl->setPos( startX, startY );
}

void input() {
	/*
	�L�[���͂ɉ�����player�̃��\�b�h���Ăяo��
	int key_input = �L�[���͂̊֐�
	float px = player->getX();
	float py = player->getY();
	float psize = player->getSize();
	
	switch ( key_input ) {
		case '' : // w, space���������ŏ�����̃u���b�N���󂷂Ƃ�
			if ( player->breakBlock( direction, map ) ) {
				map->update( px / psize, py / psize - 1, 0 );
			}
			break;

		case '' : // w, s���������ŏ�����Ƀu���b�N���Z�b�g
			if ( player->setBlock() ) {
				map->update
			}
	}
	*/
}

