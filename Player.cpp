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

	direction = 1; // 1: �E, -1: ��
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
	
	// �����^�C�v�̃u���b�N�������Ă��Ȃ����T���D
	for ( std::vector<Block*> blocks : blocksList ) {
		for ( Block* b : blocks ) {
			// �����^�C�v�̃u���b�N������΂��̎���vector�ɒǉ�����
			if ( b->getMaxDurability() == block->getMaxDurability() && b->getImageType() == block->getImageType() ) {
				blocks.push_back( block );
				return;
			}
		}
	}

	// �莝���ɓ����^�C�v�̃u���b�N���Ȃ������Ƃ��͐V�����C���̃u���b�N��p��vector�𐶐����Ēǉ�
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
	// �����Ă���u���b�N�̕\��


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