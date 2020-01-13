#include "DxLib.h"
#include "Player.h"
#include "NormalBlock.h"
#include "Loader.h"

#include <string>

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
	int i = 0;
	for ( i = 0; i < ( int )blocksList.size(); i++ ) {
		if ( blocksList.at( i ).size() >= 1 ) {
			if ( blocksList.at( i ).at( 0 )->getMaxDurability() == block->getMaxDurability() && blocksList.at( i ).at( 0 )->getImageType() == block->getImageType() ) {
				blocksList.at( i ).push_back( block );
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

	// 持っているブロックの表示
	DrawBox( 24, 16, 92, 84, GetColor( 255, 255, 255 ), TRUE );
	DrawBox( 26, 18, 90, 82, GetColor( 0, 0, 0 ), TRUE );
	DrawTriangleAA( 20.0F, 40.0F, 20.0F, 59.0F, 4.0F, 49.5F, GetColor( 240, 240, 240 ), TRUE );
	DrawTriangleAA( 96.0F, 40.0F, 96.0F, 59.0F, 112.0F, 49.5F, GetColor( 240, 240, 240 ), TRUE );
	if ( blocksList.size() >= 1 ) {
		blocksList.at( selectedItemNum ).at( 0 )->setPos( 42.0F + 32.0F * 0.5F, 34.0F + 32.0F * 0.5F );
		blocksList.at( selectedItemNum ).at( 0 )->draw( 0.0F, 0.0F, Loader::imageHandles[blocksList.at( selectedItemNum ).at( 0 )->getImageType()] );

		std::string str = std::to_string( ( int )blocksList.at( selectedItemNum ).size() );
		const char* block_num = str.c_str();
		DrawFormatString( 61, 65, GetColor( 255, 255, 255 ), "%3s", block_num );
	}
}