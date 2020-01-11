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

	// �v���C���[�̏㉺���E�̓����蔻��i����Ƃ������[�̍��W�j
	float plT = player->getTop();
	float plB = player->getBottom();
	float plR = player->getRight();
	float plL = player->getLeft();

	float cSize = map->CHIP_SIZE;

	//�S�[���n�_�ɂ��邩����
	if ( map->isGoal( plX, plY ) ) {
		return NULL;
	}

	attackBlock( plX, plY, cSize );

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
	map->draw();
	player->draw();
}

void GameScene::attackBlock( float plX, float plY , float cSize ) {
	/*
	�u���b�N����
	�w�肵�����W�Ƀu���b�N�����邩�`�F�b�N�C����Αϋv�l�����炷
	�ϋv�l��0�ɂȂ����u���b�N���}�b�v��������C�v���C���[�Ɏ�������
	*/
	if ( KeyBoard::key[KEY_INPUT_C] >= 1 ) {
		float targetX, targetY; // �ڕW�u���b�N�̒��S���W
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
	//�L�[���͂��󂯎��
		//�L�[���͂ɉ����ăv���C���[�̍s���i�ړ��C�u���b�N�ւ̑���j������
	// �ړ��ʏ�����
	dx = dy = 0.0F;
	if ( KeyBoard::key[KEY_INPUT_LEFT] >= 1 ) dx -= player->getSpeed();
	if ( KeyBoard::key[KEY_INPUT_RIGHT] >= 1 ) dx += player->getSpeed();
	if ( KeyBoard::key[KEY_INPUT_SPACE] >= 1 && ( map->hitCheck( plR, plB + 1.0F ) || map->hitCheck( plL, plB + 1.0F ) ) ) jumpSpeed -= player->getJumpPower();

	// y�����̓����蔻��
	if ( !( map->hitCheck( plR, plB + 1.0F ) || map->hitCheck( plL, plB + 1.0F ) ) ) {
		jumpSpeed += g;
	}
	dy = jumpSpeed;

	// �����
	if ( dy < 0 ) {
		if ( map->hitCheck( plR, plT + dy ) || map->hitCheck( plL, plT + dy ) ) {
			float block_bottom = ( plT + dy < 0.0F ) ? 0.0F : ( float )( ( int )( ( plT + dy ) / cSize ) + 1 ) * cSize;
			dy = block_bottom - plT + 1.0F;
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

		printfDx( "�X�e�[�W���F%s\n", str.c_str() );

		std::streampos startPos = in.tellg();
		
		// �X�g���[���̓ǂݍ��݈ʒu�������������������ړ������ʒu�ɕύX
		//in.seekg( 0, std::ifstream::end );
		in.seekg( 0, in.end );
		
		// �X�g���[���̓ǂݍ��݈ʒu���f�[�^�̖����ɗ��Ă���̂�tellg()���g�����ƂŃf�[�^�̃T�C�Y���킩��
		*size = static_cast< int >( in.tellg() );// -static_cast< int >( startPos );

		//printfDx( "�X�e�[�W�T�C�Y�F%d\n", *size );
		
		in.clear();
		in.seekg( 0, in.beg );
		//in.seekg( 0, std::ifstream::beg );
		//in.seekg( startPos, std::ifstream::beg );
		*buffer = new char[*size];
		in.read( *buffer, *size );
		
	}
}
*/