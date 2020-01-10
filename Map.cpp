#include "DxLib.h"
#include "Map.h"
#include "NormalBlock.h"
#include "UnbreakableBlock.h"

#include <algorithm>

Map::Map( const char* stageData, int fileSize ) {
	// マップで使用するオブジェクトの画像データ読み込み
	imageHandles[0] = LoadGraph( "Asset/block0.png" );
	imageHandles[1] = LoadGraph( "Asset/unbreakable_block.png" );

	setSize( stageData, fileSize ); // width, heightを初期化
	mapObjects = new MapObject[width * height];
	
	for ( int y = 0; y < height; y++ ) {
		for ( int x = 0; x < width; x++ ) {
			mapObjects[y * width + x] = OBJ_BLOCK;
		}
	}

	int y = 0;
	int x = 0;
	for ( int i = 0; i < fileSize; i++ ) {
		MapObject o;

		switch ( stageData[i] ) {
			case '#': o = OBJ_BLOCK; blocks.push_back( new UnbreakableBlock( x, y, 1 ) ); break;
			case '0': o = OBJ_BREAKABLE_BLOCK; blocks.push_back( new NormalBlock( x, y, 0 ) ); break;
			case ' ': o = OBJ_SPACE; break;
			case 'p': o = OBJ_SPACE; startX = x; startY = y; break;
			case '\n': x = 0; y++; o = OBJ_UNKNOWN; break;

			default: o = OBJ_UNKNOWN; break;
		}

		if ( o != OBJ_UNKNOWN ) {
			if ( y >= height ) y = height - 1;
			if ( x >= width ) x = width - 1;
			mapObjects[y * width + x] = o;
			x++;
		}
	}
}

void Map::setSize( const char* stageData, int fileSize ) {
	width = height = 0;

	int y = 0;
	int x = 0;

	for ( int i = 0; i < fileSize; i++ ) {
		switch ( stageData[i] ) {
			case '#': case ' ': case 'b':
				x++;
				break;

			case '\n':
				y++;
				width = ( x > width ) ? x : width;
				height = ( y > height ) ? y : height;
				x = 0;
				break;
		}
	}
}

std::tuple< int, int > Map::getStartPos() {
	return { startX, startY };
}

void Map::update( int tx, int ty, int type ) {
	//state[ty * width + tx] = type;

	//createObject( type );
}

bool Map::hitCheck( int x, int y ) {
	if ( mapObjects[y * width + x] > 0 ) {
		return true;
	} else return false;
}

void Map::createObject( int type ) {

}

void Map::draw() {
	for ( Block* block : blocks ) {
		block->draw( imageHandles[block->getImageType()] );
	}
}
