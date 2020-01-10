#include "DxLib.h"
#include "GameScene.h"
#include "Map.h"
#include "Player.h"

#include <iostream>
#include <string>
#include <fstream>
#include <iterator>
#include <algorithm>

GameScene::GameScene( const char *filename ) {
	std::string* data;
	const char* stageData;
	int fileSize;

	readFile( &data, &fileSize, filename );
	//readFile( &stageData, &fileSize, filename );

	stageData = data->c_str();

	map = new Map( stageData, data->size() );
	//maps.push_back( map );

	int image = LoadGraph( "Asset/player.png" );
	player = new Player( image );

	//delete data;
	//delete[] stageData;
}

void GameScene::init() {
	int plX, plY;
	std::tie( plX, plY ) = map->getStartPos();
	player->setPos( plX, plY );
}

Scene* GameScene::update() {
	if ( CheckHitKey( KEY_INPUT_Z ) != 0 ) {
		return NULL;
	} else return this;
}

void GameScene::draw() {
	map->draw();
	player->draw();
}

void GameScene::input() {

}

void GameScene::readFile( std::string** buffer, int* size, const char* filename ) {
	std::ifstream in( filename );
	if ( !in ) {
		*buffer = 0;
		*size = 0;
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