#include "DxLib.h"
#include "Player.h"
#include "NormalBlock.h"
#include "SpringBlock.h"
#include "UnbreakableBlock.h"
#include "KeyBoard.h"
#include "Loader.h"

#include <string>

Player::Player() : power( 2 ) {

	x = 0.0F;
	y = 0.0F;
	size = 32.0F;
	speed = 0.5F;
	jumpPower = 5.0F;
	selectedItemNum = 0;

	direction = 1; // 1: 右, -1: 左
	
	attackFlag = false;
	attackAnimationCount = 0;
	
	moveFlag = false;
	moveAnimationCount = 0;
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

Block* Player::putBlock( float x, float y ) {
	if ( blocksList.size() == 0 ) return nullptr;

	//int maxDurability = blocksList.at( selectedItemNum ).back()->getMaxDurability();
	int imageType = blocksList.at( selectedItemNum ).back()->getImageType();
	Block* block = 0;
	if ( imageType == Loader::SPRING_BLOCK ) {
		SpringBlock* sb = dynamic_cast< SpringBlock* >( blocksList.at( selectedItemNum ).back() );
		if ( sb != nullptr ) block = new SpringBlock( 0, 0, imageType, sb->getSpringCoefficient(), sb->getSpringPower() );
	} else block = new UnbreakableBlock( 0, 0, imageType );

	block->setPos( x, y );
	
	delete blocksList.at( selectedItemNum ).back();
	blocksList.at( selectedItemNum ).pop_back();

	if ( blocksList.at( selectedItemNum ).size() == 0 ) { // 設置したことで特定の種類のブロックがなくなったら
		blocksList.erase( blocksList.begin() + selectedItemNum ); // その種類用のvectorを削除

		if ( blocksList.size() == 0 ) selectedItemNum = 0;
		else if ( selectedItemNum == blocksList.size() ) {
			selectedItemNum -= 1;
		} else selectedItemNum = selectedItemNum % blocksList.size();
	}

	return block;
}

void Player::getBlock( Block* targetBlock ) {
	//int maxDurability = targetBlock->getMaxDurability();
	int imageType = targetBlock->getImageType();
	//Block* block = new NormalBlock( 0, 0, maxDurability, imageType );
	
	Block* block = 0;
	if ( imageType == Loader::SPRING_BLOCK ) {
		SpringBlock* sb = dynamic_cast< SpringBlock* >( targetBlock );
		if ( sb != nullptr ) block = new SpringBlock( 0, 0, imageType, sb->getSpringCoefficient(), sb->getSpringPower() );
	} else block = new UnbreakableBlock( 0, 0, imageType - 1 );
	// 同じタイプのブロックを持っていないか探す．
	/*
	for ( std::vector<Block*> blocks : blocksList ) {
		if ( blocks.size() >= 1 ) {
			if ( blocks.back()->getMaxDurability() == block->getMaxDurability() && blocks.back()->getImageType() == block->getImageType() ) {
				
			}
		}
	}
	*/
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

void Player::setMoveFlag( bool move ) {
	moveFlag = move;
}

void Player::changeSelectedItemNum() {
	if ( blocksList.size() == 0 ) return;
	if ( KeyBoard::key[KEY_INPUT_S] == 1 ) { // アイテムカーソルを左に移動
		if ( selectedItemNum == 0 ) selectedItemNum = ( int )blocksList.size() - 1;
		else selectedItemNum -= 1;
	} else if ( KeyBoard::key[KEY_INPUT_D] == 1 ) { // アイテムカーソルを右に移動
		selectedItemNum = ( selectedItemNum + 1 ) % ( int )blocksList.size();
	}
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

	if ( attackFlag ) { // 攻撃アニメーション
		int index = 0;
		if ( attackAnimationCount >= 0 && attackAnimationCount < 5 ) index = 0;
		else index = 1;
		if ( direction > 0 ) DrawGraph( ( int )( X - size * 0.5F ), ( int )( Y - size * 0.5F ), Loader::imageHandles[Loader::RIGHT_ATTACK_1 + index], TRUE );
		else if ( direction < 0 ) DrawGraph( ( int )( X - size * 0.5F ), ( int )( Y - size * 0.5F ), Loader::imageHandles[Loader::LEFT_ATTACK_1 + index], TRUE );

		attackAnimationCount = ( attackAnimationCount + 1 ) % 20;
		if ( attackAnimationCount == 0 ) attackFlag = false;
	} else if ( moveFlag ) { // 移動アニメーション
		if ( direction > 0 ) DrawGraph( ( int )( X - size * 0.5F ), ( int )( Y - size * 0.5F ), Loader::imageHandles[Loader::RIGHT_WALK_1 + moveAnimationCount / 12], TRUE );
		else if ( direction < 0 ) DrawGraph( ( int )( X - size * 0.5F ), ( int )( Y - size * 0.5F ), Loader::imageHandles[Loader::LEFT_WALK_1 + moveAnimationCount / 12], TRUE );

		moveAnimationCount = ( moveAnimationCount + 1 ) % 24;
	} else { // 通常描画
		if ( direction > 0 ) DrawGraph( ( int )( X - size * 0.5F ), ( int )( Y - size * 0.5F ), Loader::imageHandles[Loader::RIGHT], TRUE );
		else if ( direction < 0 ) DrawGraph( ( int )( X - size * 0.5F ), ( int )( Y - size * 0.5F ), Loader::imageHandles[Loader::LEFT], TRUE );
	}

	// 持っているブロックの表示
	DrawBox( 24, 16, 92, 84, GetColor( 255, 255, 255 ), TRUE );
	DrawBox( 26, 18, 90, 82, GetColor( 0, 0, 0 ), TRUE );
	DrawTriangleAA( 20.0F, 40.0F, 20.0F, 59.0F, 4.0F, 49.5F, GetColor( 240, 240, 240 ), TRUE );
	DrawTriangleAA( 96.0F, 40.0F, 96.0F, 59.0F, 112.0F, 49.5F, GetColor( 240, 240, 240 ), TRUE );
	if ( blocksList.size() >= 1 ) {
		if ( blocksList.at( selectedItemNum ).size() == 0 ) return;
		blocksList.at( selectedItemNum ).at( 0 )->setPos( 42.0F + 32.0F * 0.5F, 34.0F + 32.0F * 0.5F );
		blocksList.at( selectedItemNum ).at( 0 )->draw( 0.0F, 0.0F, Loader::imageHandles[blocksList.at( selectedItemNum ).at( 0 )->getImageType()] );

		std::string str = std::to_string( ( int )blocksList.at( selectedItemNum ).size() );
		const char* block_num = str.c_str();
		DrawFormatString( 61, 65, GetColor( 255, 255, 255 ), "%3s", block_num );
	}
}