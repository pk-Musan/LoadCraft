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
	//maps.push_back( map );

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

	//ゴール地点にいるか判定
	if ( map->isGoal( plX, plY ) ) {
		return NULL;
	}

	attackBlock( plX, plY, cSize );

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
	map->draw();
	player->draw();
}

void GameScene::attackBlock( float plX, float plY , float cSize ) {
	/*
	ブロックを壊す
	指定した座標にブロックがあるかチェック，あれば耐久値を減らす
	耐久値が0になったブロックをマップから消去，プレイヤーに持たせる
	*/
	if ( KeyBoard::key[KEY_INPUT_C] >= 1 ) {
		float targetX, targetY; // 目標ブロックの中心座標
		targetX = ( float )( ( int )( ( plX + cSize ) / cSize ) * cSize + cSize * 0.5F );
		targetY = ( float )( ( int )( plY / cSize ) * cSize + cSize * 0.5F );

		Block* b = map->getBlock( targetX, targetY );
		if ( b != nullptr ) {
			b->attacked( player->getPower() );

			if ( b->isBroken() ) {
				map->eraseBlock( targetX, targetY );
			}
		}
	}
}

void GameScene::charaMove( float plT, float plB, float plL, float plR, float cSize ) {
	//キー入力を受け取る
		//キー入力に応じてプレイヤーの行動（移動，ブロックへの操作）を決定
	// 移動量初期化
	dx = dy = 0.0F;
	if ( KeyBoard::key[KEY_INPUT_LEFT] >= 1 ) dx -= player->getSpeed();
	if ( KeyBoard::key[KEY_INPUT_RIGHT] >= 1 ) dx += player->getSpeed();
	if ( KeyBoard::key[KEY_INPUT_SPACE] >= 1 && ( map->hitCheck( plR, plB + 1.0F ) || map->hitCheck( plL, plB + 1.0F ) ) ) jumpSpeed -= player->getJumpPower();

	// y方向の当たり判定
	if ( !( map->hitCheck( plR, plB + 1.0F ) || map->hitCheck( plL, plB + 1.0F ) ) ) {
		jumpSpeed += g;
	}
	dy = jumpSpeed;

	// 上方向
	if ( dy < 0 ) {
		if ( map->hitCheck( plR, plT + dy ) || map->hitCheck( plL, plT + dy ) ) {
			float block_bottom = ( plT + dy < 0.0F ) ? 0.0F : ( float )( ( int )( ( plT + dy ) / cSize ) + 1 ) * cSize;
			dy = block_bottom - plT + 1.0F;
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

/*
void GameScene::readFile( const char** buffer, int* size, const char* filename ) {
	std::ifstream in( filename );
	if ( !in ) {
		*buffer = 0;
		*size = 0;
	} else {

		std::string str;
		std::getline( in, str );

		printfDx( "ステージ情報：%s\n", str.c_str() );

		std::streampos startPos = in.tellg();
		
		// ストリームの読み込み位置を第二引数から第一引数分移動した位置に変更
		//in.seekg( 0, std::ifstream::end );
		in.seekg( 0, in.end );
		
		// ストリームの読み込み位置がデータの末尾に来ているのでtellg()を使うことでデータのサイズがわかる
		*size = static_cast< int >( in.tellg() );// -static_cast< int >( startPos );

		//printfDx( "ステージサイズ：%d\n", *size );
		
		in.clear();
		in.seekg( 0, in.beg );
		//in.seekg( 0, std::ifstream::beg );
		//in.seekg( startPos, std::ifstream::beg );
		*buffer = new char[*size];
		in.read( *buffer, *size );
		
	}
}
*/