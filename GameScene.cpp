#include "DxLib.h"
#include "GameScene.h"
#include "Map.h"
#include "Player.h"
#include "KeyBoard.h"

#include <iostream>
#include <string>
#include <fstream>
#include <iterator>
#include <algorithm>

GameScene::GameScene( const char *filename ) {
	std::string* stageData;

	readFile( &stageData, filename );

	map = new Map( stageData->c_str(), stageData->size() );

	player = new Player();
	dx = dy = 0.0F;
	jumpSpeed = 0.0F;
	g = 0.5F;

	delete stageData;
}

GameScene::~GameScene() {
	delete player;
	player = 0;

	delete map;
	map = 0;
}

void GameScene::init() {
	int plX = map->getStartX();
	int plY = map->getStartY();

	player->setPos( plX, plY );
	cameraX = player->getX() - ( 640.0F * 0.5F );
	cameraY = player->getY() - ( 480.0F * 0.5F );
}

Scene* GameScene::update() {
	float plX = player->getX();
	float plY = player->getY();

	// プレイヤーの上下左右の当たり判定（判定というか端の座標）
	float plT = player->getTop();
	float plB = player->getBottom();
	float plR = player->getRight();
	float plL = player->getLeft();

	float cSize = map->CHIP_SIZE;

	// ゴール地点にいるか判定
	if ( map->isGoal( plX, plY ) ) {
		return NULL;
	}

	// 攻撃したブロックが壊れているか
	if ( targetBlock != nullptr && !player->isAttacking() ) {
		if ( targetBlock->isBroken() ) {
			player->getBlock( targetBlock->getMaxDurability(), targetBlock->getImageType() );
		}
	}

	// プレイヤーが攻撃中でない
	if ( !player->isAttacking() ) {
		targetBlock = attackBlock( plX, plY, cSize );
	}

	// 壊れている，かつアニメーションが終わっているブロックだけマップから削除
	map->eraseBlock();

	/*
	ブロックを設置
	ブロックを持っているか判定
	指定した座標にブロックがないかチェック，なければ設置
	プレイヤーの持ち物からブロックを減らす
	*/

	charaMove( plT, plB, plL, plR, cSize );
	
	return this;
}

void GameScene::draw() {
	map->draw( cameraX, cameraY );
	player->draw( cameraX, cameraY );
}

Block* GameScene::attackBlock( float plX, float plY , float cSize ) {
	/*
	ブロックを攻撃
	指定した座標にブロックがあるかチェック，あれば耐久値を減らす
	耐久値が0になったブロックをマップから消去，プレイヤーに持たせる
	*/
	float targetX, targetY; // 目標ブロックの中心座標
	Block* b = nullptr;

	if ( KeyBoard::key[KEY_INPUT_C] == 1 && KeyBoard::key[KEY_INPUT_UP] == 0 && KeyBoard::key[KEY_INPUT_DOWN] == 0 ) {
		int dir = player->getDirection();
		targetX = ( float )( ( int )( ( plX + dir * cSize ) / cSize ) * cSize + cSize * 0.5F );
		targetY = ( float )( ( int )( plY / cSize ) * cSize + cSize * 0.5F );
		b = map->getBlock( targetX, targetY );
		player->attack();
	} else if ( KeyBoard::key[KEY_INPUT_C] == 1 && KeyBoard::key[KEY_INPUT_UP] >= 1 && KeyBoard::key[KEY_INPUT_DOWN] == 0 ) {
		targetX = ( float )( ( int )( plX / cSize ) * cSize + cSize * 0.5F );
		targetY = ( float )( ( int )( ( plY - cSize ) / cSize ) * cSize + cSize * 0.5F );
		b = map->getBlock( targetX, targetY );
		player->attack();
	} else if ( KeyBoard::key[KEY_INPUT_C] == 1 && KeyBoard::key[KEY_INPUT_UP] == 0 && KeyBoard::key[KEY_INPUT_DOWN] >= 1 ) {
		targetX = ( float )( ( int )( plX / cSize ) * cSize + cSize * 0.5F );
		targetY = ( float )( ( int )( ( plY + cSize ) / cSize ) * cSize + cSize * 0.5F );
		b = map->getBlock( targetX, targetY );
		player->attack();
	}

	if ( b != nullptr ) {
		//printfDx( "%f : %f\n", b->getX(), b->getY() );
		b->attacked( player->getPower() );
	}

	return b;
}

void GameScene::charaMove( float plT, float plB, float plL, float plR, float cSize ) {
	//キー入力を受け取る
		//キー入力に応じてプレイヤーの行動（移動，ブロックへの操作）を決定
	// 移動量初期化
	dx = dy = 0.0F;
	if ( KeyBoard::key[KEY_INPUT_LEFT] >= 1 ) {
		dx -= player->getSpeed();
		player->setDirection( -1 );
	}
	if ( KeyBoard::key[KEY_INPUT_RIGHT] >= 1 ) {
		dx += player->getSpeed();
		player->setDirection( 1 );
	}
	if ( KeyBoard::key[KEY_INPUT_SPACE] == 1 && ( map->hitCheck( plR, plB + 1.0F ) || map->hitCheck( plL, plB + 1.0F ) ) ) jumpSpeed -= player->getJumpPower();

	// y方向の当たり判定
	if ( !( map->hitCheck( plR, plB + 1.0F ) || map->hitCheck( plL, plB + 1.0F ) ) ) { // プレイヤーの下にブロックがない
		jumpSpeed += g;
	}
	dy = jumpSpeed;

	if ( dy < 0 ) { // 上方向
		if ( map->hitCheck( plR, plT + dy ) || map->hitCheck( plL, plT + dy ) ) {
			// 真上のブロックの下辺
			float block_bottom = ( plT + dy < 0.0F ) ? 0.0F : ( float )( ( int )( ( plT + dy ) / cSize ) + 1 ) * cSize;
			dy = block_bottom - plT + 1.0F; // 自キャラの上側の当たり判定 から 真上のブロックの下辺 までの 距離（1.0F分考慮しないと埋まる）
			jumpSpeed *= -1.0F;
		}
	} else if ( dy > 0 ) { // 下方向
		if ( map->hitCheck( plR, plB + dy ) || map->hitCheck( plL, plB + dy ) ) {
			float block_top = ( float )( ( int )( ( plB + dy ) / cSize ) ) * cSize;
			dy = block_top - plB - 1.0F;
			jumpSpeed = 0.0F;
		}
	}

	// y方向の移動量を加算
	player->moveY( dy );
	cameraY = player->getY() - ( 480.0F * 0.5F );
	if ( cameraY < 0.0F ) cameraY = 0.0F;
	else if ( cameraY + 480.0F > ( float )( map->getHeight() * map->CHIP_SIZE ) ) {
		cameraY = ( float )( map->getHeight() * map->CHIP_SIZE ) - 480.0F;
	}

	// x方向の当たり判定
	if ( dx < 0 ) {
		if ( map->hitCheck( plL + dx, plT ) || map->hitCheck( plL + dx, plB ) ) {
			float block_right = ( plL + dx < 0.0F ) ? 0.0F : ( float )( ( int )( ( plL + dx ) / cSize ) + 1 ) * cSize;
			dx = block_right - plL + 1.0F;
		}
	} else if ( dx > 0 ) {
		if ( map->hitCheck( plR + dx, plT ) || map->hitCheck( plR + dx, plB ) ) {
			//右側のブロックの左辺
			float block_left = ( float )( ( int )( ( plR + dx ) / cSize ) ) * cSize;
			dx = block_left - plR - 1.0F;
		}
	}

	// x方向の移動量を加算
	player->moveX( dx );
	cameraX = player->getX() - ( 640.0F * 0.5F );
	if ( cameraX < 0.0F ) cameraX = 0.0F;
	else if ( cameraX + 640.0F > ( float )( map->getWidth() * map->CHIP_SIZE ) ) {
		cameraX = ( float )( map->getWidth() * map->CHIP_SIZE ) - 640.0F;
	}
}

void GameScene::readFile( std::string** buffer, const char* filename ) {
	std::ifstream in( filename );
	if ( !in ) {
		*buffer = 0;
	} else {
		std::istreambuf_iterator<char> it( in );
		std::istreambuf_iterator<char> last;
		//std::string str( it, last );

		*buffer = new std::string(it, last);
	}
}