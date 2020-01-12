#include "DxLib.h"
#include "Player.h"
#include "NormalBlock.h"

Player::Player() : power( 2 ) {
	imageHandles[RIGHT] = LoadGraph( "Asset/player_right.png" );
	imageHandles[RIGHT_ATTACK_1] = LoadGraph( "Asset/player_right_attack1.png" );
	imageHandles[RIGHT_ATTACK_2] = LoadGraph( "Asset/player_right_attack2.png" );
	imageHandles[LEFT] = LoadGraph( "Asset/player_left.png" );
	imageHandles[LEFT_ATTACK_1] = LoadGraph( "Asset/player_left_attack1.png" );
	imageHandles[LEFT_ATTACK_2] = LoadGraph( "Asset/player_left_attack2.png" );

	x = 0.0F;
	y = 0.0F;
	size = 32.0F;
	speed = 2.0F;
	jumpPower = 6.0F;
	selectedItemNum = blocks.size();

	direction = 1; // 1: 右, -1: 左
	attackFlag = false;
	attackAnimationCount = 0;
}

Player::~Player() {
	for ( Block* b : blocks ) {
		delete b;
		b = 0;
	}

	for ( int i = 0; i < PLAYER_MAX_INDEX; i++ ) {
		DeleteGraph( imageHandles[i] );
	}
}

void Player::setPos( int sx, int sy ) {
	x = ( float )( sx * size + size * 0.5F );
	y = ( float )( sy * size + size * 0.5F );
}

/*
std::tuple< int, int > Player::attack( int dx, int dy ) {
	return { ( int )( x / size ) + dx, ( int )( y / size ) + dy };
}

std::tuple< int, int > Player::setBlock( int dx, int dy ) {
	return { x + dx, y + dy };
}
*/

void Player::getBlock( int maxDurability, int imageType ) {
	Block* block = new NormalBlock( 0, 0, maxDurability, imageType );
	blocks.push_back( block );
}

void Player::setDirection( int dir ) {
	direction = dir;
}

void Player::attack() {
	attackFlag = true;
}

void Player::moveX( float dx ) {
	x += dx;
}

void Player::moveY( float dy ) {
	y += dy;
}

void Player::draw( float cameraX, float cameraY ) {
	// 持っているブロックの表示
	float X, Y;
	X = x - cameraX;
	Y = y - cameraY;

	if ( attackFlag ) {
		if ( direction > 0 ) {
			DrawGraph( ( int )( X - size * 0.5F ), ( int )( Y - size * 0.5F ), imageHandles[RIGHT + attackAnimationCount / 6], TRUE );
		} else if ( direction < 0 ) {
			DrawGraph( ( int )( X - size * 0.5F ), ( int )( Y - size * 0.5F ), imageHandles[LEFT + attackAnimationCount / 6], TRUE );
		}
		attackAnimationCount++;
		if ( attackAnimationCount >= 18 ) {
			attackFlag = false;
			attackAnimationCount = 0;
		}
	} else {
		if ( direction > 0 ) DrawGraph( ( int )( X - size * 0.5F ), ( int )( Y - size * 0.5F ), imageHandles[RIGHT], TRUE );
		else if ( direction < 0 ) DrawGraph( ( int )( X - size * 0.5F ), ( int )( Y - size * 0.5F ), imageHandles[LEFT], TRUE );
	}
}