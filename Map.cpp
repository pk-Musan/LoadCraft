#include "DxLib.h"
#include "Map.h"
#include "NormalBlock.h"
#include "UnbreakableBlock.h"
#include "DisappearingBlock.h"
#include "SpringBlock.h"
#include "Loader.h"

#include <algorithm>


Map::Map( const char* stageData, int fileSize ) : CHIP_SIZE( 32.0F ), startX( 0 ), startY( 0 ), goalX( 0 ), goalY( 0 ) {
	
	setSize( stageData, fileSize ); // width, heightを初期化
	mapObjects = new MapObject[width * height];

	for ( int y = 0; y < height; y++ ) {
		for ( int x = 0; x < width; x++ ) {
			mapObjects[y * width + x] = MapObject::OBJ_BLOCK;
		}
	}

	int y = 0;
	int x = 0;
	for ( int i = 0; i < fileSize; i++ ) {
		MapObject o;

		switch ( stageData[i] ) {
			case '#': o = MapObject::OBJ_UNBREAKABLE_BLOCK; blocks.push_back( new UnbreakableBlock( x, y, Loader::UNBREAKABLE_BLOCK ) ); break;
			case 'd': o = MapObject::OBJ_UNBREAKABLE_BLOCK; blocks.push_back( new DisappearingBlock( x, y, Loader::DISAPPEARING_BLOCK ) ); break;
			case 's': o = MapObject::OBJ_SPRING_BLOCK; blocks.push_back( new SpringBlock( x, y, Loader::SPRING_BLOCK, 0.9F, 3.0F ) ); break;
			case '1': o = MapObject::OBJ_BLOCK; blocks.push_back( new NormalBlock( x, y, 1, Loader::BLOCK_1_CRACKED ) ); break;
			case '2': o = MapObject::OBJ_BLOCK; blocks.push_back( new NormalBlock( x, y, 5, Loader::BLOCK_2_CRACKED ) ); break;
			case ' ': o = MapObject::OBJ_SPACE; break;
			case 'p': o = MapObject::OBJ_SPACE; startX = x; startY = y; break;
			case 'w': o = MapObject::OBJ_WARP; warps.push_back( new Warp( x, y ) ); break;
			case 'g': o = MapObject::OBJ_GOAL; goalX = x; goalY = y; break;
			case '\n': x = 0; y++; o = MapObject::OBJ_UNKNOWN; break;

			default: o = MapObject::OBJ_UNBREAKABLE_BLOCK; blocks.push_back( new UnbreakableBlock( x, y, Loader::UNBREAKABLE_BLOCK ) ); break;
		}

		if ( o != MapObject::OBJ_UNKNOWN ) {
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

	for ( Warp* w : warps ) {
		delete w;
		w = 0;
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

Map::MapObject Map::getMapChip( float tx, float ty ) {
	if ( tx < 0.0F || ty < 0.0F || tx >= ( float )width * CHIP_SIZE || ty >= ( float )height * CHIP_SIZE ) {
		return MapObject::OBJ_UNKNOWN;
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

void Map::putBlock( Block* block ) {
	int x = ( int )( block->getX() / CHIP_SIZE );
	int y = ( int )( block->getY() / CHIP_SIZE );

	if ( block->getImageType() == Loader::SPRING_BLOCK ) {
		mapObjects[y * width + x] = MapObject::OBJ_SPRING_BLOCK;
	} else mapObjects[y * width + x] = MapObject::OBJ_UNBREAKABLE_BLOCK;

	blocks.push_back( block );
}

void Map::eraseBlock() {
	for ( int itr = blocks.size()-1; itr >= 0; itr-- ) {
		if ( blocks.at( itr )->isBroken() && blocks.at( itr )->brokenAnimationCount >= 24 ) {
			int x = ( int )( blocks.at( itr )->getX() / CHIP_SIZE );
			int y = ( int )( blocks.at( itr )->getY() / CHIP_SIZE );
			mapObjects[y * width + x] = MapObject::OBJ_SPACE;

			delete blocks.at( itr );
			blocks.erase( blocks.begin() + itr );
		}
	}
}

bool Map::canPutBlock( float x, float y ) {
	if ( getMapChip( x, y ) != MapObject::OBJ_SPACE ) return false;

	MapObject bL, bR, bT, bB;
	bL = getMapChip( x - CHIP_SIZE, y );
	bR = getMapChip( x + CHIP_SIZE, y );
	bT = getMapChip( x, y - CHIP_SIZE );
	bB = getMapChip( x, y + CHIP_SIZE );


	if ( getMapChip( x - CHIP_SIZE, y ) == MapObject::OBJ_BLOCK || getMapChip( x - CHIP_SIZE, y ) == MapObject::OBJ_UNBREAKABLE_BLOCK || getMapChip( x - CHIP_SIZE, y ) == MapObject::OBJ_SPRING_BLOCK ) return true;
	if ( getMapChip( x + CHIP_SIZE, y ) == MapObject::OBJ_BLOCK || getMapChip( x + CHIP_SIZE, y ) == MapObject::OBJ_UNBREAKABLE_BLOCK || getMapChip( x + CHIP_SIZE, y ) == MapObject::OBJ_SPRING_BLOCK ) return true;
	if ( getMapChip( x, y - CHIP_SIZE ) == MapObject::OBJ_BLOCK || getMapChip( x, y - CHIP_SIZE ) == MapObject::OBJ_UNBREAKABLE_BLOCK || getMapChip( x, y - CHIP_SIZE ) == MapObject::OBJ_SPRING_BLOCK ) return true;
	if ( getMapChip( x, y + CHIP_SIZE ) == MapObject::OBJ_BLOCK || getMapChip( x, y + CHIP_SIZE ) == MapObject::OBJ_UNBREAKABLE_BLOCK || getMapChip( x, y + CHIP_SIZE ) == MapObject::OBJ_SPRING_BLOCK ) return true;

	return false;
}

bool Map::hitCheck( float x, float y ) {
	MapObject m = getMapChip( x, y );
	if ( m != MapObject::OBJ_SPACE && m != MapObject::OBJ_WARP && m != MapObject::OBJ_GOAL ) return true;
	else return false;
}

bool Map::isBlock( float x, float y ) {
	//if ( getMapChip( x, y ) == MapObject::OBJ_BLOCK ) return true;
	if ( getMapChip( x, y ) == MapObject::OBJ_BLOCK || getMapChip( x, y ) == MapObject::OBJ_UNBREAKABLE_BLOCK || getMapChip( x, y ) == MapObject::OBJ_SPRING_BLOCK ) return true;
	else return false;
}

bool Map::isWarp( float plX, float plY ) {
	int x = ( int )( plX / CHIP_SIZE );
	int y = ( int )( plY / CHIP_SIZE );

	if ( mapObjects[y * width + x] == MapObject::OBJ_WARP ) return true;
	else return false;
}

bool Map::isGoal( float plX, float plY ) {
	int x = ( int )( plX / CHIP_SIZE );
	int y = ( int )( plY / CHIP_SIZE );

	if ( mapObjects[y * width + x] == MapObject::OBJ_GOAL ) return true;
	else return false;
}

void Map::draw( float cameraX, float cameraY, bool stop ) {

	// ブロックの描画
	for ( Block* block : blocks ) {
		float bL, bR, bT, bB;
		bL = block->getX() - CHIP_SIZE * 0.5F;
		bR = block->getX() + CHIP_SIZE * 0.5F - 1.0F;
		bT = block->getY() - CHIP_SIZE * 0.5F;
		bB = block->getY() + CHIP_SIZE * 0.5F - 1.0F;

		DisappearingBlock* dBlock = dynamic_cast< DisappearingBlock* >( block );

		if ( block->isBroken() ) { // 壊れたブロックの描画
			if ( !( bR < cameraX || bL > cameraX + 640.0F - 1.0F || bB < cameraY || bT > cameraY + 480.0F - 1.0F ) ) {
				block->draw( cameraX, cameraY, Loader::imageHandles[block->getImageType() + block->brokenAnimationCount / 12] );
			}
			if ( !stop ) block->brokenAnimationCount++;
		} else {
			if ( dBlock != nullptr ) {
				if ( !( bR < cameraX || bL > cameraX + 640.0F - 1.0F || bB < cameraY || bT > cameraY + 480.0F - 1.0F ) ) {
					if ( dBlock->isVisible() ) {
						block->draw( cameraX, cameraY, Loader::imageHandles[block->getImageType()] );
					}
					if ( !stop ) dBlock->incrementVisibleCount();
				}
				//if ( !stop ) dBlock->incrementVisibleCount();
			} else {
				if ( !( bR < cameraX || bL > cameraX + 640.0F - 1.0F || bB < cameraY || bT > cameraY + 480.0F - 1.0F ) ) {
					block->draw( cameraX, cameraY, Loader::imageHandles[block->getImageType()] );
				}
			}
		}
		
	}

	// ゴールの描画
	float gL, gR, gT, gB;
	gL = ( float )goalX * CHIP_SIZE;
	gR = ( float )( goalX + 1 ) * CHIP_SIZE - 1.0F;
	gT = ( float )goalY * CHIP_SIZE;
	gB = ( float )( goalY + 1 ) * CHIP_SIZE - 1.0F;
	if ( !( gR < cameraX || gL > cameraX + 640.0F - 1.0F || gB < cameraY || gT > cameraY + 480.0F - 1.0F ) ) {
		DrawGraph( ( int )( goalX * CHIP_SIZE - cameraX ), ( int )( goalY * CHIP_SIZE - cameraY ), Loader::imageHandles[Loader::GOAL], TRUE );
	}

	// ワープポイントの描画
	for ( Warp* w : warps ) {
		int wX = w->getX();
		int wY = w->getY();

		float wL, wR, wT, wB;
		wL = ( float )wX * CHIP_SIZE;
		wR = ( float )( wX + 1 ) * CHIP_SIZE - 1.0F;
		wT = ( float )wY * CHIP_SIZE;
		wB = ( float )( wY + 1 ) * CHIP_SIZE - 1.0F;

		if ( !( wR < cameraX || wL > cameraX + 640.0F - 1.0F || wB < cameraY || wT > cameraY + 480.0F - 1.0F ) ) {
			DrawGraph( ( int )( wX * CHIP_SIZE - cameraX ), ( int )( wY * CHIP_SIZE - cameraY ), Loader::imageHandles[Loader::WARP_1 + w->getAnimationCount() / 20], TRUE );
		}
		if ( !stop ) w->incrementAnimationCount();
	}
	/*
	float wL, wR, wT, wB;
	wL = ( float )warpX * CHIP_SIZE;
	wR = ( float )( warpX + 1 ) * CHIP_SIZE - 1.0F;
	wT = ( float )warpY * CHIP_SIZE;
	wB = ( float )( warpY + 1 ) * CHIP_SIZE - 1.0F;
	if ( !( wR < cameraX || wL > cameraX + 640.0F - 1.0F || wB < cameraY || wT > cameraY + 480.0F - 1.0F ) ) {
		DrawGraph( ( int )( warpX * CHIP_SIZE - cameraX ), ( int )( warpY * CHIP_SIZE - cameraY ), Loader::imageHandles[Loader::WARP_1 + warpAnimationCount / 20], TRUE );
	}
	if ( !stop ) warpAnimationCount = ( warpAnimationCount + 1 ) % 40;
	*/
}
