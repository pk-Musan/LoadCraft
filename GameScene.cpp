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

	// �v���C���[�̏㉺���E�̓����蔻��i����Ƃ������[�̍��W�j
	float plT = player->getTop();
	float plB = player->getBottom();
	float plR = player->getRight();
	float plL = player->getLeft();

	float cSize = map->CHIP_SIZE;

	// �S�[���n�_�ɂ��邩����
	if ( map->isGoal( plX, plY ) ) {
		return NULL;
	}

	// �U�������u���b�N�����Ă��邩
	if ( targetBlock != nullptr && !player->isAttacking() ) {
		if ( targetBlock->isBroken() ) {
			player->getBlock( targetBlock->getMaxDurability(), targetBlock->getImageType() );
		}
	}

	// �v���C���[���U�����łȂ�
	if ( !player->isAttacking() ) {
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

	charaMove( plT, plB, plL, plR, cSize );
	
	return this;
}

void GameScene::draw() {
	map->draw( cameraX, cameraY );
	player->draw( cameraX, cameraY );
}

Block* GameScene::attackBlock( float plX, float plY , float cSize ) {
	/*
	�u���b�N���U��
	�w�肵�����W�Ƀu���b�N�����邩�`�F�b�N�C����Αϋv�l�����炷
	�ϋv�l��0�ɂȂ����u���b�N���}�b�v��������C�v���C���[�Ɏ�������
	*/
	float targetX, targetY; // �ڕW�u���b�N�̒��S���W
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
	//�L�[���͂��󂯎��
		//�L�[���͂ɉ����ăv���C���[�̍s���i�ړ��C�u���b�N�ւ̑���j������
	// �ړ��ʏ�����
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

	// y�����̓����蔻��
	if ( !( map->hitCheck( plR, plB + 1.0F ) || map->hitCheck( plL, plB + 1.0F ) ) ) { // �v���C���[�̉��Ƀu���b�N���Ȃ�
		jumpSpeed += g;
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
	cameraY = player->getY() - ( 480.0F * 0.5F );
	if ( cameraY < 0.0F ) cameraY = 0.0F;
	else if ( cameraY + 480.0F > ( float )( map->getHeight() * map->CHIP_SIZE ) ) {
		cameraY = ( float )( map->getHeight() * map->CHIP_SIZE ) - 480.0F;
	}

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