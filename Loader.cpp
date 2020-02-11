#include "DxLib.h"
#include "Loader.h"

int Loader::imageHandles[MAX_INDEX];

void Loader::loadGraph() {
	imageHandles[TITLE] = LoadGraph( "Asset/title.png" );
	imageHandles[PRESS_1] = LoadGraph( "Asset/press1.png" );
	imageHandles[PRESS_2] = LoadGraph( "Asset/press2.png" );
	imageHandles[ANY_KEY_1] = LoadGraph( "Asset/any_key1.png" );
	imageHandles[ANY_KEY_2] = LoadGraph( "Asset/any_key2.png" );

	imageHandles[RIGHT] = LoadGraph( "Asset/player_right.png" );
	imageHandles[RIGHT_WALK_1] = LoadGraph( "Asset/player_right_walk1.png" );
	imageHandles[RIGHT_WALK_2] = LoadGraph( "Asset/player_right_walk2.png" );
	imageHandles[RIGHT_ATTACK_1] = LoadGraph( "Asset/player_right_attack1.png" );
	imageHandles[RIGHT_ATTACK_2] = LoadGraph( "Asset/player_right_attack2.png" );

	imageHandles[LEFT] = LoadGraph( "Asset/player_left.png" );
	imageHandles[LEFT_WALK_1] = LoadGraph( "Asset/player_left_walk1.png" );
	imageHandles[LEFT_WALK_2] = LoadGraph( "Asset/player_left_walk2.png" );
	imageHandles[LEFT_ATTACK_1] = LoadGraph( "Asset/player_left_attack1.png" );
	imageHandles[LEFT_ATTACK_2] = LoadGraph( "Asset/player_left_attack2.png" );

	imageHandles[TARGET] = LoadGraph( "Asset/target.png" );

	imageHandles[BLOCK_1] = LoadGraph( "Asset/block1.png" );
	imageHandles[BLOCK_1_CRACKED] = LoadGraph( "Asset/block1_cracked.png" );
	imageHandles[BLOCK_1_BROKEN] = LoadGraph( "Asset/block1_broken.png" );

	imageHandles[BLOCK_2] = LoadGraph( "Asset/block2.png" );
	imageHandles[BLOCK_2_CRACKED] = LoadGraph( "Asset/block2_cracked.png" );
	imageHandles[BLOCK_2_BROKEN] = LoadGraph( "Asset/block2_broken.png" );

	imageHandles[SPRING_BLOCK] = LoadGraph( "Asset/spring_block.png" );
	imageHandles[SPRING_BLOCK_BROKEN] = LoadGraph( "Asset/spring_block_broken.png" );

	imageHandles[DISAPPEARING_BLOCK] = LoadGraph( "Asset/disappearing_block.png" );

	imageHandles[UNBREAKABLE_BLOCK] = LoadGraph( "Asset/unbreakable_block.png" );

	imageHandles[WARP_1] = LoadGraph( "Asset/warp1.png" );
	imageHandles[WARP_2] = LoadGraph( "Asset/warp2.png" );
	imageHandles[GOAL] = LoadGraph( "Asset/goal.png" );

	imageHandles[ARROW] = LoadGraph( "Asset/arrow.png" );
}

void Loader::deleteGraph() {
	for ( int i = 0; i < MAX_INDEX; i++ ) {
		DeleteGraph( imageHandles[i] );
	}
}