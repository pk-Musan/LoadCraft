#include "GameScene.h"
#include "Map.h"
#include "Player.h"

GameScene::GameScene( char *filename ) {
	Map *map = new Map( filename );
	maps.push_back( map );

	player = new Player();
}

void GameScene::init() {
	int plX, plY;
	std::tie( plX, plY ) = maps.at( 0 )->getStartPos();
	player->setPos( plX, plY );
}

Scene* GameScene::update() {

}

void GameScene::draw() {

}

void GameScene::input() {

}