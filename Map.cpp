#include "DxLib.h"
#include "Map.h"
#include "NormalBlock.h"
#include "UnbreakableBlock.h"

#include <algorithm>


Map::Map( const char* stageData, int fileSize ) : CHIP_SIZE( 32.0F ), startX( 0 ), startY( 0 ), goalX( 0 ), goalY( 0 ) {
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
			case '#': o = OBJ_UNBREAKABLE_BLOCK; blocks.push_back( new UnbreakableBlock( x, y, 1 ) ); break;
			case '0': o = OBJ_BLOCK; blocks.push_back( new NormalBlock( x, y, 0 ) ); break;
			case ' ': o = OBJ_SPACE; break;
			case 'p': o = OBJ_SPACE; startX = x; startY = y; break;
			case 'g': o = OBJ_SPACE; goalX = x; goalY = y; break;
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

Map::~Map() {
	delete[] mapObjects;
	mapObjects = 0;

	for ( Block* b : blocks ) {
		delete b;
		b = 0;
	}

	for ( int i = 0; i < 2; i++ ) {
		DeleteGraph( imageHandles[i] );
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

MapObject Map::getMapChip( float tx, float ty ) {
	if ( tx < 0.0F || ty < 0.0F || tx >= ( float )width * CHIP_SIZE || ty >= ( float )height * CHIP_SIZE ) {
		return OBJ_UNBREAKABLE_BLOCK;
	} else {
		int x, y;
		x = ( int )( tx / CHIP_SIZE );
		y = ( int )( ty / CHIP_SIZE );
		return mapObjects[y * width + x];
	}
}

Block* Map::getBlock( float x, float y ) {
	if ( isBlock( x, y ) ) {
		for ( Block* b : blocks ) {
			if ( b->getX() == x && b->getY() == y ) return b;
		}
		return nullptr;
	} else return nullptr;
}

void Map::eraseBlock( float tx, float ty ) {
	int itr = 0;
	for ( Block* b : blocks ) {
		if ( b->getX() == tx && b->getY() == ty ) {
			break;
		}
		itr++;
	}
	blocks.erase( blocks.begin() + itr );

	int x = ( int )( tx / CHIP_SIZE );
	int y = ( int )( ty / CHIP_SIZE );
	mapObjects[y * width + x] = OBJ_SPACE;
}

bool Map::hitCheck( float x, float y ) {
	if ( getMapChip( x, y ) != OBJ_SPACE ) return true;
	else return false;
}

bool Map::isBlock( float x, float y ) {
	if ( getMapChip( x, y ) == OBJ_BLOCK ) return true;
	else return false;
}

bool Map::isGoal( float plX, float plY ) {
	int x = ( int )( plX / CHIP_SIZE );
	int y = ( int )( plY / CHIP_SIZE );

	if ( x == goalX && y == goalY ) return true;
	else return false;
}

void Map::createObject( int type ) {

}

void Map::draw() {
	for ( Block* block : blocks ) {
		block->draw( imageHandles[block->getImageType()] );
	}
}
