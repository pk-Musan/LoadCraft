#include "DxLib.h"
#include "GameScene.h"
#include "TitleScene.h"
#include "Map.h"
#include "Player.h"
#include "KeyBoard.h"
#include "Loader.h"
#include "Font.h"

#include <iostream>
#include <string>
#include <fstream>
#include <iterator>
#include <algorithm>

GameScene::GameScene( const char *filename ) {
	readError = false;

	std::string* stageData;
	readFile( &stageData, filename );
	
	if ( stageData == NULL ) {
		readError = true;
		delete stageData;
		return;
	}

	this->filename = filename;
	map = new Map( stageData->c_str(), stageData->size() );
	player = new Player();

	dx = dy = 0.0F;
	jumpSpeed = 0.0F;
	g = 0.4F;
	finishCount = 0;
	warpCount = 0;

	pause = false;
	pauseCursor = 0;

	delete stageData;
}

GameScene::~GameScene() {
	delete player;
	player = 0;

	delete map;
	map = 0;

	//Loader::deleteGraph();
}

void GameScene::init() {
	if ( readError ) return;

	int plX = map->getStartX();
	int plY = map->getStartY();

	player->setPos( plX, plY );
	cameraX = player->getX() - ( 640.0F * 0.5F );
	cameraY = player->getY() - ( 480.0F * 0.5F );

	if ( cameraX < 0.0F ) cameraX = 0.0F;
	else if ( cameraX + 640.0F > ( float )( map->getWidth() * map->CHIP_SIZE ) ) {
		cameraX = ( float )( map->getWidth() * map->CHIP_SIZE ) - 640.0F;
	}

	if ( cameraY < 0.0F ) cameraY = 0.0F;
	else if ( cameraY + 480.0F > ( float )( map->getHeight() * map->CHIP_SIZE ) ) {
		cameraY = ( float )( map->getHeight() * map->CHIP_SIZE ) - 480.0F;
	}
}

Scene* GameScene::update() {
	if ( readError ) {
		TitleScene* tScene = new TitleScene();
		return tScene;
	}

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
		player->setMoveFlag( false );
		if ( finishCount >= 180 && KeyBoard::key[KEY_INPUT_ESCAPE] == 1 ) {
			TitleScene* tScene = new TitleScene();
			return tScene;
		}
		return this;
	}

	if ( map->isWarp( plX, plY ) ) {
		player->setMoveFlag( false );
		if ( warpCount >= 180 ) {
			GameScene* gScene = new GameScene( filename );
			return gScene;
		}
		return this;
	}

	if ( KeyBoard::key[KEY_INPUT_ESCAPE] == 1 ) pause = true;
	if ( pause ) {
		player->setMoveFlag( false );
		if ( KeyBoard::key[KEY_INPUT_UP] == 1 ) {
			pauseCursor -= 1;
			if ( pauseCursor < 0 ) pauseCursor = 2; // 最大値を代入
		}
		if ( KeyBoard::key[KEY_INPUT_DOWN] == 1 ) pauseCursor = ( pauseCursor + 1 ) % 3;

		if ( KeyBoard::key[KEY_INPUT_SPACE] == 1 ) {
			if ( pauseCursor == 0 ) pause = false;
			else if ( pauseCursor == 1 ) {
				GameScene* gScene = new GameScene( filename );
				return gScene;
			} else if ( pauseCursor == 2 ) {
				TitleScene* tScene = new TitleScene();
				return tScene;
			}
		}
		return this;
	}

	player->changeSelectedItemNum();

	// プレイヤーが攻撃中でない
	if ( !player->isAttacking() ) {
		if ( targetBlock != nullptr ) {
			if ( targetBlock->isBroken() ) { // 壊れていればブロックを入手
				player->getBlock( targetBlock->getMaxDurability(), targetBlock->getImageType() - 1 ); // 壊したブロックを本来の状態で入手
				targetBlock = nullptr;
			}
		}
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
	putBlock( plX, plY, plL, plR, plT, plB, cSize );

	moveChara( plL, plR, plT, plB, cSize );
	
	moveCamera();

	moveTargetCursor();

	return this;
}

void GameScene::draw() {
	if ( readError ) return;

	map->draw( cameraX, cameraY );
	player->draw( cameraX, cameraY );
	if ( targetCursorX >= 0 && targetCursorY >= 0 )	DrawGraph( targetCursorX, targetCursorY, Loader::imageHandles[Loader::TARGET], TRUE );

	if ( map->isGoal( player->getX(), player->getY() ) ) {
		SetDrawBlendMode( DX_BLENDMODE_ALPHA, 150 );
		DrawBox( 0, 0, ( int )( map->getWidth() * map->CHIP_SIZE ), ( int )( map->getHeight() * map->CHIP_SIZE ), GetColor( 255, 255, 255 ), TRUE );
		SetDrawBlendMode( DX_BLENDMODE_NOBLEND, 0 );
		DrawStringToHandle( 120, 215, "ステージクリア！", GetColor( 255, 0, 0 ), Font::fonts[0] );

		if ( finishCount >= 180 ) {
			DrawStringToHandle( 390, 420, "Esc：タイトルに戻る", GetColor( 255, 0, 0 ), Font::fonts[1] );
		}
		finishCount = ( finishCount >= 180 ) ? 180 : finishCount + 1;
	}

	if ( map->isWarp( player->getX(), player->getY() ) ) {
		SetDrawBlendMode( DX_BLENDMODE_ALPHA, 255 - ( 255 - ( int )( warpCount * float( 255 / 180 ) ) ) );
		DrawBox( 0, 0, ( int )( map->getWidth() * map->CHIP_SIZE ), ( int )( map->getHeight() * map->CHIP_SIZE ), GetColor( 255, 255, 255 ), TRUE );
		SetDrawBlendMode( DX_BLENDMODE_NOBLEND, 0 );
		warpCount++;
	}

	if ( pause ) {
		SetDrawBlendMode( DX_BLENDMODE_ALPHA, 150 );
		DrawBox( 0, 0, ( int )( map->getWidth() * map->CHIP_SIZE ), ( int )( map->getHeight() * map->CHIP_SIZE ), GetColor( 0, 0, 0 ), TRUE );
		SetDrawBlendMode( DX_BLENDMODE_NOBLEND, 0 );

		int standardX, standardY;
		standardX = 400;
		standardY = 100 + pauseCursor * 80 - 1;
		
		DrawGraph( standardX, standardY, Loader::imageHandles[Loader::LEFT + cursorAnimationCount / 8], TRUE );
		DrawGraph( standardX + 32, standardY - 32, Loader::imageHandles[Loader::LEFT + cursorAnimationCount / 8], TRUE );
		DrawGraph( standardX + 32, standardY, Loader::imageHandles[Loader::LEFT + cursorAnimationCount / 8], TRUE );
		DrawGraph( standardX + 32, standardY + 32, Loader::imageHandles[Loader::LEFT + cursorAnimationCount / 8], TRUE );
		DrawGraph( standardX + 64, standardY, Loader::imageHandles[Loader::LEFT + cursorAnimationCount / 8], TRUE );
		DrawGraph( standardX + 96, standardY, Loader::imageHandles[Loader::LEFT + cursorAnimationCount / 8], TRUE );

		DrawStringToHandle( 60, 100, "ゲームに戻る", GetColor( 255, 255, 255 ), Font::fonts[2] );
		DrawStringToHandle( 60, 180, "初期位置に戻る", GetColor( 255, 255, 255 ), Font::fonts[2] );
		DrawStringToHandle( 60, 260, "タイトルに戻る", GetColor( 255, 255, 255 ), Font::fonts[2] );
		DrawStringToHandle( 160, 400, "Press Space Key", GetColor( 255, 255, 255 ), Font::fonts[2] );

		cursorAnimationCount = ( cursorAnimationCount + 1 ) % 24;
	}
}

Block* GameScene::attackBlock( float plX, float plY , float cSize ) {
	/*
	ブロックを攻撃
	指定した座標にブロックがあるかチェック，あれば耐久値を減らす
	耐久値が0になったブロックをマップから消去，プレイヤーに持たせる
	*/
	float targetX, targetY; // 目標ブロックの中心座標
	Block* b = nullptr;
	Map::MapObject mObj1, mObj2;

	if ( KeyBoard::key[KEY_INPUT_LEFT] >= 1 ) player->setDirection( -1 );
	if ( KeyBoard::key[KEY_INPUT_RIGHT] >= 1 ) player->setDirection( 1 );

	// 左シフトを押していない場合は，真横，真上，真下のみを対象
	if ( KeyBoard::key[KEY_INPUT_C] == 1 ) {
		if ( /*KeyBoard::key[KEY_INPUT_LSHIFT] >= 1 &&*/ KeyBoard::key[KEY_INPUT_LEFT] >= 1 && KeyBoard::key[KEY_INPUT_RIGHT] == 0 && KeyBoard::key[KEY_INPUT_UP] >= 1 && KeyBoard::key[KEY_INPUT_DOWN] == 0 ) { // 左上のブロック
			player->setDirection( -1 );
			player->attack();
			mObj1 = map->getMapChip( plX - cSize, plY );
			mObj2 = map->getMapChip( plX, plY - cSize );
			if ( mObj1 != Map::MapObject::OBJ_SPACE && mObj2 != Map::MapObject::OBJ_SPACE ) return nullptr;

			int dir = player->getDirection();
			targetX = ( float )( ( int )( ( plX + dir * cSize ) / cSize ) * cSize + cSize * 0.5F );
			targetY = ( float )( ( int )( ( plY - cSize ) / cSize ) * cSize + cSize * 0.5F );
			b = map->getBlock( targetX, targetY );
		} else if ( /*KeyBoard::key[KEY_INPUT_LSHIFT] >= 1 &&*/ KeyBoard::key[KEY_INPUT_LEFT] == 0 && KeyBoard::key[KEY_INPUT_RIGHT] >= 1 && KeyBoard::key[KEY_INPUT_UP] >= 1 && KeyBoard::key[KEY_INPUT_DOWN] == 0 ) { // 右上のブロック
			player->setDirection( 1 );
			player->attack();
			mObj1 = map->getMapChip( plX + cSize, plY );
			mObj2 = map->getMapChip( plX, plY - cSize );
			if ( mObj1 != Map::MapObject::OBJ_SPACE && mObj2 != Map::MapObject::OBJ_SPACE ) return nullptr;

			int dir = player->getDirection();
			targetX = ( float )( ( int )( ( plX + dir * cSize ) / cSize ) * cSize + cSize * 0.5F );
			targetY = ( float )( ( int )( ( plY - cSize ) / cSize ) * cSize + cSize * 0.5F );
			b = map->getBlock( targetX, targetY );
		} else if ( /*KeyBoard::key[KEY_INPUT_LSHIFT] >= 1 && */KeyBoard::key[KEY_INPUT_LEFT] >= 1 && KeyBoard::key[KEY_INPUT_RIGHT] == 0 && KeyBoard::key[KEY_INPUT_UP] == 0 && KeyBoard::key[KEY_INPUT_DOWN] >= 1 ) { // 左下のブロック
			player->setDirection( -1 );
			player->attack();
			mObj1 = map->getMapChip( plX - cSize, plY );
			mObj2 = map->getMapChip( plX, plY + cSize );
			if ( mObj1 != Map::MapObject::OBJ_SPACE && mObj2 != Map::MapObject::OBJ_SPACE ) return nullptr;

			int dir = player->getDirection();
			targetX = ( float )( ( int )( ( plX + dir * cSize ) / cSize ) * cSize + cSize * 0.5F );
			targetY = ( float )( ( int )( ( plY + cSize ) / cSize ) * cSize + cSize * 0.5F );
			b = map->getBlock( targetX, targetY );
		} else if ( /*KeyBoard::key[KEY_INPUT_LSHIFT] >= 1 &&*/ KeyBoard::key[KEY_INPUT_LEFT] == 0 && KeyBoard::key[KEY_INPUT_RIGHT] >= 1 && KeyBoard::key[KEY_INPUT_UP] == 0 && KeyBoard::key[KEY_INPUT_DOWN] >= 1 ) { // 右下のブロック
			player->setDirection( 1 );
			player->attack();
			mObj1 = map->getMapChip( plX + cSize, plY );
			mObj2 = map->getMapChip( plX, plY + cSize );
			if ( mObj1 != Map::MapObject::OBJ_SPACE && mObj2 != Map::MapObject::OBJ_SPACE ) return nullptr;

			int dir = player->getDirection();
			targetX = ( float )( ( int )( ( plX + dir * cSize ) / cSize ) * cSize + cSize * 0.5F );
			targetY = ( float )( ( int )( ( plY + cSize ) / cSize ) * cSize + cSize * 0.5F );
			b = map->getBlock( targetX, targetY );
		} else if ( KeyBoard::key[KEY_INPUT_UP] == 0 && KeyBoard::key[KEY_INPUT_DOWN] == 0 ) { // 真横のブロック
			int dir = player->getDirection();
			targetX = ( float )( ( int )( ( plX + dir * cSize ) / cSize ) * cSize + cSize * 0.5F );
			targetY = ( float )( ( int )( plY / cSize ) * cSize + cSize * 0.5F );
			b = map->getBlock( targetX, targetY );
			player->attack();
		} else if ( KeyBoard::key[KEY_INPUT_UP] >= 1 && KeyBoard::key[KEY_INPUT_DOWN] == 0 ) { // 真上のブロック
			targetX = ( float )( ( int )( plX / cSize ) * cSize + cSize * 0.5F );
			targetY = ( float )( ( int )( ( plY - cSize ) / cSize ) * cSize + cSize * 0.5F );
			b = map->getBlock( targetX, targetY );
			player->attack();
		} else if ( KeyBoard::key[KEY_INPUT_UP] == 0 && KeyBoard::key[KEY_INPUT_DOWN] >= 1 ) { // 真下のブロック
			targetX = ( float )( ( int )( plX / cSize ) * cSize + cSize * 0.5F );
			targetY = ( float )( ( int )( ( plY + cSize ) / cSize ) * cSize + cSize * 0.5F );
			b = map->getBlock( targetX, targetY );
			player->attack();
		}
	}

	if ( b != nullptr ) {
		//printfDx( "%f : %f\n", b->getX(), b->getY() );
		if ( b->isBroken() ) return nullptr; // 直前の攻撃で壊れていてまだ描画中ならそのブロックは返さない
		b->attacked( player->getPower() );
	}

	return b;
}

void GameScene::putBlock( float plX, float plY, float plL, float plR, float plT, float plB, float cSize ) {
	float targetX, targetY; // 目標ブロックの中心座標

	// マップのインデックスにおいてプレイヤーの端の座標が含まれる箇所の中心座標
	float leftX = ( float )( ( int )( plL / cSize ) * cSize + cSize * 0.5F );
	float rightX = ( float )( ( int )( plR / cSize ) * cSize + cSize * 0.5F );
	float topY = ( float )( ( int )( plT / cSize ) * cSize + cSize * 0.5F );
	float bottomY = ( float )( ( int )( plB / cSize ) * cSize + cSize * 0.5F );

	Map::MapObject mObj1, mObj2;

	if ( KeyBoard::key[KEY_INPUT_LEFT] >= 1 ) player->setDirection( -1 );
	if ( KeyBoard::key[KEY_INPUT_RIGHT] >= 1 ) player->setDirection( 1 );

	// 左シフトを押していない場合は，真横，真上，真下のみを対象
	if ( KeyBoard::key[KEY_INPUT_Z] == 1 ) {
		if ( /*KeyBoard::key[KEY_INPUT_LSHIFT] >= 1 &&*/ KeyBoard::key[KEY_INPUT_LEFT] >= 1 && KeyBoard::key[KEY_INPUT_RIGHT] == 0 && KeyBoard::key[KEY_INPUT_UP] >= 1 && KeyBoard::key[KEY_INPUT_DOWN] == 0 ) { // 左上のブロック
			mObj1 = map->getMapChip( plX - cSize, plY );
			mObj2 = map->getMapChip( plX, plY - cSize );
			if ( mObj1 != Map::MapObject::OBJ_SPACE && mObj2 != Map::MapObject::OBJ_SPACE ) return;

			targetX = ( float )( ( int )( ( plX - cSize ) / cSize ) * cSize + cSize * 0.5F );
			targetY = ( float )( ( int )( ( plY - cSize ) / cSize ) * cSize + cSize * 0.5F );
			if ( ( targetX == leftX && targetY == topY ) || !map->canPutBlock( targetX, targetY ) ) return; // ブロックを設置したい箇所にプレイヤーがいないか

			Block* block = player->putBlock( targetX, targetY );
			if ( block != nullptr ) map->putBlock( block );
		} else if ( /*KeyBoard::key[KEY_INPUT_LSHIFT] >= 1 && */KeyBoard::key[KEY_INPUT_LEFT] == 0 && KeyBoard::key[KEY_INPUT_RIGHT] >= 1 && KeyBoard::key[KEY_INPUT_UP] >= 1 && KeyBoard::key[KEY_INPUT_DOWN] == 0 ) { // 右上のブロック
			mObj1 = map->getMapChip( plX + cSize, plY );
			mObj2 = map->getMapChip( plX, plY - cSize );
			if ( mObj1 != Map::MapObject::OBJ_SPACE && mObj2 != Map::MapObject::OBJ_SPACE ) return;

			targetX = ( float )( ( int )( ( plX + cSize ) / cSize ) * cSize + cSize * 0.5F );
			targetY = ( float )( ( int )( ( plY - cSize ) / cSize ) * cSize + cSize * 0.5F );
			if ( ( targetX == rightX && targetY == topY ) || !map->canPutBlock( targetX, targetY ) ) return; // ブロックを設置したい箇所にプレイヤーがいないか

			Block* block = player->putBlock( targetX, targetY );
			if ( block != nullptr ) map->putBlock( block );
		} else if ( /*KeyBoard::key[KEY_INPUT_LSHIFT] >= 1 &&*/ KeyBoard::key[KEY_INPUT_LEFT] >= 1 && KeyBoard::key[KEY_INPUT_RIGHT] == 0 && KeyBoard::key[KEY_INPUT_UP] == 0 && KeyBoard::key[KEY_INPUT_DOWN] >= 1 ) { // 左下のブロック
			mObj1 = map->getMapChip( plX - cSize, plY );
			mObj2 = map->getMapChip( plX, plY + cSize );
			if ( mObj1 != Map::MapObject::OBJ_SPACE && mObj2 != Map::MapObject::OBJ_SPACE ) return;

			targetX = ( float )( ( int )( ( plX - cSize ) / cSize ) * cSize + cSize * 0.5F );
			targetY = ( float )( ( int )( ( plY + cSize ) / cSize ) * cSize + cSize * 0.5F );
			if ( ( targetX == leftX && targetY == bottomY ) || !map->canPutBlock( targetX, targetY ) ) return; // ブロックを設置したい箇所にプレイヤーがいないか

			Block* block = player->putBlock( targetX, targetY );
			if ( block != nullptr ) map->putBlock( block );
		} else if ( /*KeyBoard::key[KEY_INPUT_LSHIFT] >= 1 &&*/ KeyBoard::key[KEY_INPUT_LEFT] == 0 && KeyBoard::key[KEY_INPUT_RIGHT] >= 1 && KeyBoard::key[KEY_INPUT_UP] == 0 && KeyBoard::key[KEY_INPUT_DOWN] >= 1 ) { // 右下のブロック
			mObj1 = map->getMapChip( plX + cSize, plY );
			mObj2 = map->getMapChip( plX, plY + cSize );
			if ( mObj1 != Map::MapObject::OBJ_SPACE && mObj2 != Map::MapObject::OBJ_SPACE ) return;

			targetX = ( float )( ( int )( ( plX + cSize ) / cSize ) * cSize + cSize * 0.5F );
			targetY = ( float )( ( int )( ( plY + cSize ) / cSize ) * cSize + cSize * 0.5F );
			if ( ( targetX == rightX && targetY == bottomY ) || !map->canPutBlock( targetX, targetY ) ) return; // ブロックを設置したい箇所にプレイヤーがいないか

			Block* block = player->putBlock( targetX, targetY );
			if ( block != nullptr ) map->putBlock( block );
		} else if ( KeyBoard::key[KEY_INPUT_UP] == 0 && KeyBoard::key[KEY_INPUT_DOWN] == 0 ) { // 真横のブロック
			int dir = player->getDirection();
			targetX = ( float )( ( int )( ( plX + dir * cSize ) / cSize ) * cSize + cSize * 0.5F );
			targetY = ( float )( ( int )( plY / cSize ) * cSize + cSize * 0.5F );

			if ( ( targetX == leftX || targetX == rightX ) || !map->canPutBlock( targetX, targetY ) ) return; // ブロックを設置したい箇所にプレイヤーがいないか

			Block* block = player->putBlock( targetX, targetY );
			if ( block != nullptr ) map->putBlock( block );
		} else if ( KeyBoard::key[KEY_INPUT_UP] >= 1 && KeyBoard::key[KEY_INPUT_DOWN] == 0 ) { // 真上のブロック
			targetX = ( float )( ( int )( plX / cSize ) * cSize + cSize * 0.5F );
			targetY = ( float )( ( int )( ( plY - cSize ) / cSize ) * cSize + cSize * 0.5F );
			if ( targetY == topY || !map->canPutBlock( targetX, targetY ) ) return; // ブロックを設置したい箇所にプレイヤーがいないか

			Block* block = player->putBlock( targetX, targetY );
			if ( block != nullptr ) map->putBlock( block );
		} else if ( KeyBoard::key[KEY_INPUT_UP] == 0 && KeyBoard::key[KEY_INPUT_DOWN] >= 1 ) { // 真下のブロック
			targetX = ( float )( ( int )( plX / cSize ) * cSize + cSize * 0.5F );
			targetY = ( float )( ( int )( ( plY + cSize ) / cSize ) * cSize + cSize * 0.5F );

			if ( targetY == bottomY || !map->canPutBlock( targetX, targetY ) ) return; // ブロックを設置したい箇所にプレイヤーがいないか

			Block* block = player->putBlock( targetX, targetY );
			if ( block != nullptr ) map->putBlock( block );
		}
	}
}

void GameScene::moveChara( float plL, float plR, float plT, float plB, float cSize ) {
	//キー入力を受け取る
		//キー入力に応じてプレイヤーの行動（移動，ブロックへの操作）を決定
	// 移動量初期化
	dx = dy = 0.0F;
	if ( KeyBoard::key[KEY_INPUT_LSHIFT] == 0 ) {
		if ( KeyBoard::key[KEY_INPUT_LEFT] >= 1/* && KeyBoard::key[KEY_INPUT_LSHIFT] == 0 */) {
			dx -= player->getSpeed();
			player->setDirection( -1 );
		}
		if ( KeyBoard::key[KEY_INPUT_RIGHT] >= 1/* && KeyBoard::key[KEY_INPUT_LSHIFT] == 0 */) {
			dx += player->getSpeed();
			player->setDirection( 1 );
		}
	}
	if ( KeyBoard::key[KEY_INPUT_SPACE] == 1 && ( map->hitCheck( plR, plB + 1.0F ) || map->hitCheck( plL, plB + 1.0F ) ) ) jumpSpeed -= player->getJumpPower();

	// y方向の当たり判定
	if ( !( map->hitCheck( plR, plB + 1.0F ) || map->hitCheck( plL, plB + 1.0F ) ) ) { // プレイヤーの下にブロックがない
		jumpSpeed += g;
		if ( jumpSpeed >= 9.0F ) jumpSpeed = 9.0F;
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
	if ( dx != 0 ) player->setMoveFlag( true );
	else player->setMoveFlag( false );
}

void GameScene::moveCamera() {
	// カメラの左上のY座標を更新
	cameraY = player->getY() - ( 480.0F * 0.5F );
	if ( cameraY < 0.0F ) cameraY = 0.0F;
	else if ( cameraY + 480.0F > ( float )( map->getHeight() * map->CHIP_SIZE ) ) {
		cameraY = ( float )( map->getHeight() * map->CHIP_SIZE ) - 480.0F;
	}

	// カメラの左上のX座標を更新
	cameraX = player->getX() - ( 640.0F * 0.5F );
	if ( cameraX < 0.0F ) cameraX = 0.0F;
	else if ( cameraX + 640.0F > ( float )( map->getWidth() * map->CHIP_SIZE ) ) {
		cameraX = ( float )( map->getWidth() * map->CHIP_SIZE ) - 640.0F;
	}
}

void GameScene::moveTargetCursor() {
	float plX, plY, cSize;

	plX = player->getX();
	plY = player->getY();
	cSize = map->CHIP_SIZE;

	targetCursorX = -100;
	targetCursorY = -100;

	if ( KeyBoard::key[KEY_INPUT_LEFT] >= 1 ) player->setDirection( -1 );
	if ( KeyBoard::key[KEY_INPUT_RIGHT] >= 1 ) player->setDirection( 1 );

	if ( /*KeyBoard::key[KEY_INPUT_LSHIFT] >= 1 &&*/ KeyBoard::key[KEY_INPUT_LEFT] >= 1 && KeyBoard::key[KEY_INPUT_RIGHT] == 0 && KeyBoard::key[KEY_INPUT_UP] >= 1 && KeyBoard::key[KEY_INPUT_DOWN] == 0 ) { // 左上のブロック
		int dir = player->getDirection();
		targetCursorX = ( int )( ( int )( ( plX + dir * cSize ) / cSize ) * cSize - cameraX );
		targetCursorY = ( int )( ( int )( ( plY - cSize ) / cSize ) * cSize - cameraY );
	} else if ( /*KeyBoard::key[KEY_INPUT_LSHIFT] >= 1 &&*/ KeyBoard::key[KEY_INPUT_LEFT] == 0 && KeyBoard::key[KEY_INPUT_RIGHT] >= 1 && KeyBoard::key[KEY_INPUT_UP] >= 1 && KeyBoard::key[KEY_INPUT_DOWN] == 0 ) { // 右上のブロック
		int dir = player->getDirection();
		targetCursorX = ( int )( ( int )( ( plX + dir * cSize ) / cSize ) * cSize - cameraX );
		targetCursorY = ( int )( ( int )( ( plY - cSize ) / cSize ) * cSize - cameraY );
	} else if ( /*KeyBoard::key[KEY_INPUT_LSHIFT] >= 1 &&*/ KeyBoard::key[KEY_INPUT_LEFT] >= 1 && KeyBoard::key[KEY_INPUT_RIGHT] == 0 && KeyBoard::key[KEY_INPUT_UP] == 0 && KeyBoard::key[KEY_INPUT_DOWN] >= 1 ) { // 左下のブロック
		int dir = player->getDirection();
		targetCursorX = ( int )( ( int )( ( plX + dir * cSize ) / cSize ) * cSize - cameraX );
		targetCursorY = ( int )( ( int )( ( plY + cSize ) / cSize ) * cSize - cameraY );
	} else if ( /*KeyBoard::key[KEY_INPUT_LSHIFT] >= 1 &&*/ KeyBoard::key[KEY_INPUT_LEFT] == 0 && KeyBoard::key[KEY_INPUT_RIGHT] >= 1 && KeyBoard::key[KEY_INPUT_UP] == 0 && KeyBoard::key[KEY_INPUT_DOWN] >= 1 ) { // 右下のブロック
		player->setDirection( 1 );
		int dir = player->getDirection();
		targetCursorX = ( int )( ( int )( ( plX + dir * cSize ) / cSize ) * cSize - cameraX );
		targetCursorY = ( int )( ( int )( ( plY + cSize ) / cSize ) * cSize - cameraY );
	} else if ( KeyBoard::key[KEY_INPUT_UP] >= 1 && KeyBoard::key[KEY_INPUT_DOWN] == 0 ) { // 真上のブロック
		targetCursorX = ( int )( ( int )( plX / cSize ) * cSize - cameraX );
		targetCursorY = ( int )( ( int )( ( plY - cSize ) / cSize ) * cSize - cameraY );
	} else if ( KeyBoard::key[KEY_INPUT_UP] == 0 && KeyBoard::key[KEY_INPUT_DOWN] >= 1 ) { // 真下のブロック
		targetCursorX = ( int )( ( int )( plX / cSize ) * cSize - cameraX );
		targetCursorY = ( int )( ( int )( ( plY + cSize ) / cSize ) * cSize - cameraY );
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