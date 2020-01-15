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

	// �v���C���[�̏㉺���E�̓����蔻��i����Ƃ������[�̍��W�j
	float plT = player->getTop();
	float plB = player->getBottom();
	float plR = player->getRight();
	float plL = player->getLeft();

	float cSize = map->CHIP_SIZE;

	// �S�[���n�_�ɂ��邩����
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
			if ( pauseCursor < 0 ) pauseCursor = 2; // �ő�l����
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

	// �v���C���[���U�����łȂ�
	if ( !player->isAttacking() ) {
		if ( targetBlock != nullptr ) {
			if ( targetBlock->isBroken() ) { // ���Ă���΃u���b�N�����
				player->getBlock( targetBlock->getMaxDurability(), targetBlock->getImageType() - 1 ); // �󂵂��u���b�N��{���̏�Ԃœ���
				targetBlock = nullptr;
			}
		}
		targetBlock = attackBlock( plX, plY, cSize );
	}

	// ���Ă���C���A�j���[�V�������I����Ă���u���b�N�����}�b�v����폜
	map->eraseBlock();

	/*
	�u���b�N��ݒu
	�u���b�N�������Ă��邩����
	�w�肵�����W�Ƀu���b�N���Ȃ����`�F�b�N�C�Ȃ���ΐݒu
	�v���C���[�̎���������u���b�N�����炷
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
		DrawStringToHandle( 120, 215, "�X�e�[�W�N���A�I", GetColor( 255, 0, 0 ), Font::fonts[0] );

		if ( finishCount >= 180 ) {
			DrawStringToHandle( 390, 420, "Esc�F�^�C�g���ɖ߂�", GetColor( 255, 0, 0 ), Font::fonts[1] );
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

		DrawStringToHandle( 60, 100, "�Q�[���ɖ߂�", GetColor( 255, 255, 255 ), Font::fonts[2] );
		DrawStringToHandle( 60, 180, "�����ʒu�ɖ߂�", GetColor( 255, 255, 255 ), Font::fonts[2] );
		DrawStringToHandle( 60, 260, "�^�C�g���ɖ߂�", GetColor( 255, 255, 255 ), Font::fonts[2] );
		DrawStringToHandle( 160, 400, "Press Space Key", GetColor( 255, 255, 255 ), Font::fonts[2] );

		cursorAnimationCount = ( cursorAnimationCount + 1 ) % 24;
	}
}

Block* GameScene::attackBlock( float plX, float plY , float cSize ) {
	/*
	�u���b�N���U��
	�w�肵�����W�Ƀu���b�N�����邩�`�F�b�N�C����Αϋv�l�����炷
	�ϋv�l��0�ɂȂ����u���b�N���}�b�v��������C�v���C���[�Ɏ�������
	*/
	float targetX, targetY; // �ڕW�u���b�N�̒��S���W
	Block* b = nullptr;
	Map::MapObject mObj1, mObj2;

	if ( KeyBoard::key[KEY_INPUT_LEFT] >= 1 ) player->setDirection( -1 );
	if ( KeyBoard::key[KEY_INPUT_RIGHT] >= 1 ) player->setDirection( 1 );

	// ���V�t�g�������Ă��Ȃ��ꍇ�́C�^���C�^��C�^���݂̂�Ώ�
	if ( KeyBoard::key[KEY_INPUT_C] == 1 ) {
		if ( /*KeyBoard::key[KEY_INPUT_LSHIFT] >= 1 &&*/ KeyBoard::key[KEY_INPUT_LEFT] >= 1 && KeyBoard::key[KEY_INPUT_RIGHT] == 0 && KeyBoard::key[KEY_INPUT_UP] >= 1 && KeyBoard::key[KEY_INPUT_DOWN] == 0 ) { // ����̃u���b�N
			player->setDirection( -1 );
			player->attack();
			mObj1 = map->getMapChip( plX - cSize, plY );
			mObj2 = map->getMapChip( plX, plY - cSize );
			if ( mObj1 != Map::MapObject::OBJ_SPACE && mObj2 != Map::MapObject::OBJ_SPACE ) return nullptr;

			int dir = player->getDirection();
			targetX = ( float )( ( int )( ( plX + dir * cSize ) / cSize ) * cSize + cSize * 0.5F );
			targetY = ( float )( ( int )( ( plY - cSize ) / cSize ) * cSize + cSize * 0.5F );
			b = map->getBlock( targetX, targetY );
		} else if ( /*KeyBoard::key[KEY_INPUT_LSHIFT] >= 1 &&*/ KeyBoard::key[KEY_INPUT_LEFT] == 0 && KeyBoard::key[KEY_INPUT_RIGHT] >= 1 && KeyBoard::key[KEY_INPUT_UP] >= 1 && KeyBoard::key[KEY_INPUT_DOWN] == 0 ) { // �E��̃u���b�N
			player->setDirection( 1 );
			player->attack();
			mObj1 = map->getMapChip( plX + cSize, plY );
			mObj2 = map->getMapChip( plX, plY - cSize );
			if ( mObj1 != Map::MapObject::OBJ_SPACE && mObj2 != Map::MapObject::OBJ_SPACE ) return nullptr;

			int dir = player->getDirection();
			targetX = ( float )( ( int )( ( plX + dir * cSize ) / cSize ) * cSize + cSize * 0.5F );
			targetY = ( float )( ( int )( ( plY - cSize ) / cSize ) * cSize + cSize * 0.5F );
			b = map->getBlock( targetX, targetY );
		} else if ( /*KeyBoard::key[KEY_INPUT_LSHIFT] >= 1 && */KeyBoard::key[KEY_INPUT_LEFT] >= 1 && KeyBoard::key[KEY_INPUT_RIGHT] == 0 && KeyBoard::key[KEY_INPUT_UP] == 0 && KeyBoard::key[KEY_INPUT_DOWN] >= 1 ) { // �����̃u���b�N
			player->setDirection( -1 );
			player->attack();
			mObj1 = map->getMapChip( plX - cSize, plY );
			mObj2 = map->getMapChip( plX, plY + cSize );
			if ( mObj1 != Map::MapObject::OBJ_SPACE && mObj2 != Map::MapObject::OBJ_SPACE ) return nullptr;

			int dir = player->getDirection();
			targetX = ( float )( ( int )( ( plX + dir * cSize ) / cSize ) * cSize + cSize * 0.5F );
			targetY = ( float )( ( int )( ( plY + cSize ) / cSize ) * cSize + cSize * 0.5F );
			b = map->getBlock( targetX, targetY );
		} else if ( /*KeyBoard::key[KEY_INPUT_LSHIFT] >= 1 &&*/ KeyBoard::key[KEY_INPUT_LEFT] == 0 && KeyBoard::key[KEY_INPUT_RIGHT] >= 1 && KeyBoard::key[KEY_INPUT_UP] == 0 && KeyBoard::key[KEY_INPUT_DOWN] >= 1 ) { // �E���̃u���b�N
			player->setDirection( 1 );
			player->attack();
			mObj1 = map->getMapChip( plX + cSize, plY );
			mObj2 = map->getMapChip( plX, plY + cSize );
			if ( mObj1 != Map::MapObject::OBJ_SPACE && mObj2 != Map::MapObject::OBJ_SPACE ) return nullptr;

			int dir = player->getDirection();
			targetX = ( float )( ( int )( ( plX + dir * cSize ) / cSize ) * cSize + cSize * 0.5F );
			targetY = ( float )( ( int )( ( plY + cSize ) / cSize ) * cSize + cSize * 0.5F );
			b = map->getBlock( targetX, targetY );
		} else if ( KeyBoard::key[KEY_INPUT_UP] == 0 && KeyBoard::key[KEY_INPUT_DOWN] == 0 ) { // �^���̃u���b�N
			int dir = player->getDirection();
			targetX = ( float )( ( int )( ( plX + dir * cSize ) / cSize ) * cSize + cSize * 0.5F );
			targetY = ( float )( ( int )( plY / cSize ) * cSize + cSize * 0.5F );
			b = map->getBlock( targetX, targetY );
			player->attack();
		} else if ( KeyBoard::key[KEY_INPUT_UP] >= 1 && KeyBoard::key[KEY_INPUT_DOWN] == 0 ) { // �^��̃u���b�N
			targetX = ( float )( ( int )( plX / cSize ) * cSize + cSize * 0.5F );
			targetY = ( float )( ( int )( ( plY - cSize ) / cSize ) * cSize + cSize * 0.5F );
			b = map->getBlock( targetX, targetY );
			player->attack();
		} else if ( KeyBoard::key[KEY_INPUT_UP] == 0 && KeyBoard::key[KEY_INPUT_DOWN] >= 1 ) { // �^���̃u���b�N
			targetX = ( float )( ( int )( plX / cSize ) * cSize + cSize * 0.5F );
			targetY = ( float )( ( int )( ( plY + cSize ) / cSize ) * cSize + cSize * 0.5F );
			b = map->getBlock( targetX, targetY );
			player->attack();
		}
	}

	if ( b != nullptr ) {
		//printfDx( "%f : %f\n", b->getX(), b->getY() );
		if ( b->isBroken() ) return nullptr; // ���O�̍U���ŉ��Ă��Ă܂��`�撆�Ȃ炻�̃u���b�N�͕Ԃ��Ȃ�
		b->attacked( player->getPower() );
	}

	return b;
}

void GameScene::putBlock( float plX, float plY, float plL, float plR, float plT, float plB, float cSize ) {
	float targetX, targetY; // �ڕW�u���b�N�̒��S���W

	// �}�b�v�̃C���f�b�N�X�ɂ����ăv���C���[�̒[�̍��W���܂܂��ӏ��̒��S���W
	float leftX = ( float )( ( int )( plL / cSize ) * cSize + cSize * 0.5F );
	float rightX = ( float )( ( int )( plR / cSize ) * cSize + cSize * 0.5F );
	float topY = ( float )( ( int )( plT / cSize ) * cSize + cSize * 0.5F );
	float bottomY = ( float )( ( int )( plB / cSize ) * cSize + cSize * 0.5F );

	Map::MapObject mObj1, mObj2;

	if ( KeyBoard::key[KEY_INPUT_LEFT] >= 1 ) player->setDirection( -1 );
	if ( KeyBoard::key[KEY_INPUT_RIGHT] >= 1 ) player->setDirection( 1 );

	// ���V�t�g�������Ă��Ȃ��ꍇ�́C�^���C�^��C�^���݂̂�Ώ�
	if ( KeyBoard::key[KEY_INPUT_Z] == 1 ) {
		if ( /*KeyBoard::key[KEY_INPUT_LSHIFT] >= 1 &&*/ KeyBoard::key[KEY_INPUT_LEFT] >= 1 && KeyBoard::key[KEY_INPUT_RIGHT] == 0 && KeyBoard::key[KEY_INPUT_UP] >= 1 && KeyBoard::key[KEY_INPUT_DOWN] == 0 ) { // ����̃u���b�N
			mObj1 = map->getMapChip( plX - cSize, plY );
			mObj2 = map->getMapChip( plX, plY - cSize );
			if ( mObj1 != Map::MapObject::OBJ_SPACE && mObj2 != Map::MapObject::OBJ_SPACE ) return;

			targetX = ( float )( ( int )( ( plX - cSize ) / cSize ) * cSize + cSize * 0.5F );
			targetY = ( float )( ( int )( ( plY - cSize ) / cSize ) * cSize + cSize * 0.5F );
			if ( ( targetX == leftX && targetY == topY ) || !map->canPutBlock( targetX, targetY ) ) return; // �u���b�N��ݒu�������ӏ��Ƀv���C���[�����Ȃ���

			Block* block = player->putBlock( targetX, targetY );
			if ( block != nullptr ) map->putBlock( block );
		} else if ( /*KeyBoard::key[KEY_INPUT_LSHIFT] >= 1 && */KeyBoard::key[KEY_INPUT_LEFT] == 0 && KeyBoard::key[KEY_INPUT_RIGHT] >= 1 && KeyBoard::key[KEY_INPUT_UP] >= 1 && KeyBoard::key[KEY_INPUT_DOWN] == 0 ) { // �E��̃u���b�N
			mObj1 = map->getMapChip( plX + cSize, plY );
			mObj2 = map->getMapChip( plX, plY - cSize );
			if ( mObj1 != Map::MapObject::OBJ_SPACE && mObj2 != Map::MapObject::OBJ_SPACE ) return;

			targetX = ( float )( ( int )( ( plX + cSize ) / cSize ) * cSize + cSize * 0.5F );
			targetY = ( float )( ( int )( ( plY - cSize ) / cSize ) * cSize + cSize * 0.5F );
			if ( ( targetX == rightX && targetY == topY ) || !map->canPutBlock( targetX, targetY ) ) return; // �u���b�N��ݒu�������ӏ��Ƀv���C���[�����Ȃ���

			Block* block = player->putBlock( targetX, targetY );
			if ( block != nullptr ) map->putBlock( block );
		} else if ( /*KeyBoard::key[KEY_INPUT_LSHIFT] >= 1 &&*/ KeyBoard::key[KEY_INPUT_LEFT] >= 1 && KeyBoard::key[KEY_INPUT_RIGHT] == 0 && KeyBoard::key[KEY_INPUT_UP] == 0 && KeyBoard::key[KEY_INPUT_DOWN] >= 1 ) { // �����̃u���b�N
			mObj1 = map->getMapChip( plX - cSize, plY );
			mObj2 = map->getMapChip( plX, plY + cSize );
			if ( mObj1 != Map::MapObject::OBJ_SPACE && mObj2 != Map::MapObject::OBJ_SPACE ) return;

			targetX = ( float )( ( int )( ( plX - cSize ) / cSize ) * cSize + cSize * 0.5F );
			targetY = ( float )( ( int )( ( plY + cSize ) / cSize ) * cSize + cSize * 0.5F );
			if ( ( targetX == leftX && targetY == bottomY ) || !map->canPutBlock( targetX, targetY ) ) return; // �u���b�N��ݒu�������ӏ��Ƀv���C���[�����Ȃ���

			Block* block = player->putBlock( targetX, targetY );
			if ( block != nullptr ) map->putBlock( block );
		} else if ( /*KeyBoard::key[KEY_INPUT_LSHIFT] >= 1 &&*/ KeyBoard::key[KEY_INPUT_LEFT] == 0 && KeyBoard::key[KEY_INPUT_RIGHT] >= 1 && KeyBoard::key[KEY_INPUT_UP] == 0 && KeyBoard::key[KEY_INPUT_DOWN] >= 1 ) { // �E���̃u���b�N
			mObj1 = map->getMapChip( plX + cSize, plY );
			mObj2 = map->getMapChip( plX, plY + cSize );
			if ( mObj1 != Map::MapObject::OBJ_SPACE && mObj2 != Map::MapObject::OBJ_SPACE ) return;

			targetX = ( float )( ( int )( ( plX + cSize ) / cSize ) * cSize + cSize * 0.5F );
			targetY = ( float )( ( int )( ( plY + cSize ) / cSize ) * cSize + cSize * 0.5F );
			if ( ( targetX == rightX && targetY == bottomY ) || !map->canPutBlock( targetX, targetY ) ) return; // �u���b�N��ݒu�������ӏ��Ƀv���C���[�����Ȃ���

			Block* block = player->putBlock( targetX, targetY );
			if ( block != nullptr ) map->putBlock( block );
		} else if ( KeyBoard::key[KEY_INPUT_UP] == 0 && KeyBoard::key[KEY_INPUT_DOWN] == 0 ) { // �^���̃u���b�N
			int dir = player->getDirection();
			targetX = ( float )( ( int )( ( plX + dir * cSize ) / cSize ) * cSize + cSize * 0.5F );
			targetY = ( float )( ( int )( plY / cSize ) * cSize + cSize * 0.5F );

			if ( ( targetX == leftX || targetX == rightX ) || !map->canPutBlock( targetX, targetY ) ) return; // �u���b�N��ݒu�������ӏ��Ƀv���C���[�����Ȃ���

			Block* block = player->putBlock( targetX, targetY );
			if ( block != nullptr ) map->putBlock( block );
		} else if ( KeyBoard::key[KEY_INPUT_UP] >= 1 && KeyBoard::key[KEY_INPUT_DOWN] == 0 ) { // �^��̃u���b�N
			targetX = ( float )( ( int )( plX / cSize ) * cSize + cSize * 0.5F );
			targetY = ( float )( ( int )( ( plY - cSize ) / cSize ) * cSize + cSize * 0.5F );
			if ( targetY == topY || !map->canPutBlock( targetX, targetY ) ) return; // �u���b�N��ݒu�������ӏ��Ƀv���C���[�����Ȃ���

			Block* block = player->putBlock( targetX, targetY );
			if ( block != nullptr ) map->putBlock( block );
		} else if ( KeyBoard::key[KEY_INPUT_UP] == 0 && KeyBoard::key[KEY_INPUT_DOWN] >= 1 ) { // �^���̃u���b�N
			targetX = ( float )( ( int )( plX / cSize ) * cSize + cSize * 0.5F );
			targetY = ( float )( ( int )( ( plY + cSize ) / cSize ) * cSize + cSize * 0.5F );

			if ( targetY == bottomY || !map->canPutBlock( targetX, targetY ) ) return; // �u���b�N��ݒu�������ӏ��Ƀv���C���[�����Ȃ���

			Block* block = player->putBlock( targetX, targetY );
			if ( block != nullptr ) map->putBlock( block );
		}
	}
}

void GameScene::moveChara( float plL, float plR, float plT, float plB, float cSize ) {
	//�L�[���͂��󂯎��
		//�L�[���͂ɉ����ăv���C���[�̍s���i�ړ��C�u���b�N�ւ̑���j������
	// �ړ��ʏ�����
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

	// y�����̓����蔻��
	if ( !( map->hitCheck( plR, plB + 1.0F ) || map->hitCheck( plL, plB + 1.0F ) ) ) { // �v���C���[�̉��Ƀu���b�N���Ȃ�
		jumpSpeed += g;
		if ( jumpSpeed >= 9.0F ) jumpSpeed = 9.0F;
	}
	dy = jumpSpeed;

	if ( dy < 0 ) { // �����
		if ( map->hitCheck( plR, plT + dy ) || map->hitCheck( plL, plT + dy ) ) {
			// �^��̃u���b�N�̉���
			float block_bottom = ( plT + dy < 0.0F ) ? 0.0F : ( float )( ( int )( ( plT + dy ) / cSize ) + 1 ) * cSize;
			dy = block_bottom - plT + 1.0F; // ���L�����̏㑤�̓����蔻�� ���� �^��̃u���b�N�̉��� �܂ł� �����i1.0F���l�����Ȃ��Ɩ��܂�j
			jumpSpeed *= -1.0F;
		}
	} else if ( dy > 0 ) { // ������
		if ( map->hitCheck( plR, plB + dy ) || map->hitCheck( plL, plB + dy ) ) {
			float block_top = ( float )( ( int )( ( plB + dy ) / cSize ) ) * cSize;
			dy = block_top - plB - 1.0F;
			jumpSpeed = 0.0F;
		}
	}

	// y�����̈ړ��ʂ����Z
	player->moveY( dy );

	// x�����̓����蔻��
	if ( dx < 0 ) {
		if ( map->hitCheck( plL + dx, plT ) || map->hitCheck( plL + dx, plB ) ) {
			float block_right = ( plL + dx < 0.0F ) ? 0.0F : ( float )( ( int )( ( plL + dx ) / cSize ) + 1 ) * cSize;
			dx = block_right - plL + 1.0F;
		}
	} else if ( dx > 0 ) {
		if ( map->hitCheck( plR + dx, plT ) || map->hitCheck( plR + dx, plB ) ) {
			//�E���̃u���b�N�̍���
			float block_left = ( float )( ( int )( ( plR + dx ) / cSize ) ) * cSize;
			dx = block_left - plR - 1.0F;
		}
	}

	// x�����̈ړ��ʂ����Z
	player->moveX( dx );
	if ( dx != 0 ) player->setMoveFlag( true );
	else player->setMoveFlag( false );
}

void GameScene::moveCamera() {
	// �J�����̍����Y���W���X�V
	cameraY = player->getY() - ( 480.0F * 0.5F );
	if ( cameraY < 0.0F ) cameraY = 0.0F;
	else if ( cameraY + 480.0F > ( float )( map->getHeight() * map->CHIP_SIZE ) ) {
		cameraY = ( float )( map->getHeight() * map->CHIP_SIZE ) - 480.0F;
	}

	// �J�����̍����X���W���X�V
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

	if ( /*KeyBoard::key[KEY_INPUT_LSHIFT] >= 1 &&*/ KeyBoard::key[KEY_INPUT_LEFT] >= 1 && KeyBoard::key[KEY_INPUT_RIGHT] == 0 && KeyBoard::key[KEY_INPUT_UP] >= 1 && KeyBoard::key[KEY_INPUT_DOWN] == 0 ) { // ����̃u���b�N
		int dir = player->getDirection();
		targetCursorX = ( int )( ( int )( ( plX + dir * cSize ) / cSize ) * cSize - cameraX );
		targetCursorY = ( int )( ( int )( ( plY - cSize ) / cSize ) * cSize - cameraY );
	} else if ( /*KeyBoard::key[KEY_INPUT_LSHIFT] >= 1 &&*/ KeyBoard::key[KEY_INPUT_LEFT] == 0 && KeyBoard::key[KEY_INPUT_RIGHT] >= 1 && KeyBoard::key[KEY_INPUT_UP] >= 1 && KeyBoard::key[KEY_INPUT_DOWN] == 0 ) { // �E��̃u���b�N
		int dir = player->getDirection();
		targetCursorX = ( int )( ( int )( ( plX + dir * cSize ) / cSize ) * cSize - cameraX );
		targetCursorY = ( int )( ( int )( ( plY - cSize ) / cSize ) * cSize - cameraY );
	} else if ( /*KeyBoard::key[KEY_INPUT_LSHIFT] >= 1 &&*/ KeyBoard::key[KEY_INPUT_LEFT] >= 1 && KeyBoard::key[KEY_INPUT_RIGHT] == 0 && KeyBoard::key[KEY_INPUT_UP] == 0 && KeyBoard::key[KEY_INPUT_DOWN] >= 1 ) { // �����̃u���b�N
		int dir = player->getDirection();
		targetCursorX = ( int )( ( int )( ( plX + dir * cSize ) / cSize ) * cSize - cameraX );
		targetCursorY = ( int )( ( int )( ( plY + cSize ) / cSize ) * cSize - cameraY );
	} else if ( /*KeyBoard::key[KEY_INPUT_LSHIFT] >= 1 &&*/ KeyBoard::key[KEY_INPUT_LEFT] == 0 && KeyBoard::key[KEY_INPUT_RIGHT] >= 1 && KeyBoard::key[KEY_INPUT_UP] == 0 && KeyBoard::key[KEY_INPUT_DOWN] >= 1 ) { // �E���̃u���b�N
		player->setDirection( 1 );
		int dir = player->getDirection();
		targetCursorX = ( int )( ( int )( ( plX + dir * cSize ) / cSize ) * cSize - cameraX );
		targetCursorY = ( int )( ( int )( ( plY + cSize ) / cSize ) * cSize - cameraY );
	} else if ( KeyBoard::key[KEY_INPUT_UP] >= 1 && KeyBoard::key[KEY_INPUT_DOWN] == 0 ) { // �^��̃u���b�N
		targetCursorX = ( int )( ( int )( plX / cSize ) * cSize - cameraX );
		targetCursorY = ( int )( ( int )( ( plY - cSize ) / cSize ) * cSize - cameraY );
	} else if ( KeyBoard::key[KEY_INPUT_UP] == 0 && KeyBoard::key[KEY_INPUT_DOWN] >= 1 ) { // �^���̃u���b�N
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