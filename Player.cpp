#include "DxLib.h"
#include "Player.h"
#include "NormalBlock.h"
#include "Loader.h"

Player::Player() : power( 2 ) {

	x = 0.0F;
	y = 0.0F;
	size = 32.0F;
	speed = 2.0F;
	jumpPower = 6.0F;
	selectedItemNum = 0;

	direction = 1; // 1: 右, -1: 左
	attackFlag = false;
	attackAnimationCount = 0;
}

Player::~Player() {
	for ( std::vector<Block*> blocks : blocksList ) {
		for ( Block* b : blocks ) {
			delete b;
			b = 0;
		}
	}
}

void Player::setPos( int sx, int sy ) {
	x = ( float )( sx * size + size * 0.5F );
	y = ( float )( sy * size + size * 0.5F );
}

void Player::getBlock( int maxDurability, int imageType ) {
	Block* block = new NormalBlock( 0, 0, maxDurability, imageType );
	
	// 同じタイプのブロックを持っていないか探す．
	for ( std::vector<Block*> blocks : blocksList ) {
		for ( Block* b : blocks ) {
			// 同じタイプのブロックがあればその時のvectorに追加する
			if ( b->getMaxDurability() == block->getMaxDurability() && b->getImageType() == block->getImageType() ) {
				blocks.push_back( block );
				return;
			}
		}
	}

	// 手持ちに同じタイプのブロックがなかったときは新しく，そのブロック専用のvectorを生成して追加
	std::vector<Block*> blocks;
	blocks.push_back( block );
	blocksList.push_back( blocks );
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
			DrawGraph( ( int )( X - size * 0.5F ), ( int )( Y - size * 0.5F ), Loader::imageHandles[Loader::RIGHT + attackAnimationCount / 6], TRUE );
		} else if ( direction < 0 ) {
			DrawGraph( ( int )( X - size * 0.5F ), ( int )( Y - size * 0.5F ), Loader::imageHandles[Loader::LEFT + attackAnimationCount / 6], TRUE );
		}
		attackAnimationCount++;
		if ( attackAnimationCount >= 18 ) {
			attackFlag = false;
			attackAnimationCount = 0;
		}
	} else {
		if ( direction > 0 ) DrawGraph( ( int )( X - size * 0.5F ), ( int )( Y - size * 0.5F ), Loader::imageHandles[Loader::RIGHT], TRUE );
		else if ( direction < 0 ) DrawGraph( ( int )( X - size * 0.5F ), ( int )( Y - size * 0.5F ), Loader::imageHandles[Loader::LEFT], TRUE );
	}
}