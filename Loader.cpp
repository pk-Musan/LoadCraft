#include "DxLib.h"
#include "Loader.h"

int Loader::imageHandles[MAX_INDEX];

void Loader::loadGraph() {
	imageHandles[RIGHT] = LoadGraph( "Asset/player_right.png" );
	imageHandles[RIGHT_ATTACK_1] = LoadGraph( "Asset/player_right_attack1.png" );
	imageHandles[RIGHT_ATTACK_2] = LoadGraph( "Asset/player_right_attack2.png" );
	imageHandles[LEFT] = LoadGraph( "Asset/player_left.png" );
	imageHandles[LEFT_ATTACK_1] = LoadGraph( "Asset/player_left_attack1.png" );
	imageHandles[LEFT_ATTACK_2] = LoadGraph( "Asset/player_left_attack2.png" );

	imageHandles[BLOCK] = LoadGraph( "Asset/block0.png" );
	imageHandles[BLOCK_BROKEN] = LoadGraph( "Asset/block0_broken.png" );
	imageHandles[UNBREAKABLE_BLOCK] = LoadGraph( "Asset/unbreakable_block.png" );
}

void Loader::deleteGraph() {
	for ( int i = 0; i < MAX_INDEX; i++ ) {
		DeleteGraph( imageHandles[i] );
	}
}