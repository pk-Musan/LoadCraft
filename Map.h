#ifndef _MAP_H_
#define _MAP_H_

#include "Block.h"
#include <vector>

enum MapObject {
	OBJ_SPACE,
	OBJ_BLOCK,
	OBJ_UNBREAKABLE_BLOCK,
	OBJ_PLAYER,

	OBJ_UNKNOWN
};

enum BlockImage {
	BLOCK,
	BLOCK_BROKEN,

	UNBREAKABLE_BLOCK,

	BLOCK_MAX_INDEX
};

class Map {
private:
	MapObject* mapObjects = 0;
	int width;
	int height;
	int startX, startY;
	int goalX, goalY;
	std::vector<Block*> blocks;
	int imageHandles[BLOCK_MAX_INDEX];

public:
	const float CHIP_SIZE;

public:
	Map( const char* stageData, int fileSize );
	~Map();
	void setSize( const char* stageData, int fileSeize );

	int getStartX() { return startX; }
	int getStartY() { return startY; }
	int getWidth() { return width; }
	int getHeight() { return height; }
	MapObject getMapChip( float tx, float ty );
	Block* getBlock( float x, float y );

	void eraseBlock( /*float x, float y*/ );
	bool hitCheck( float x, float y );
	bool isBlock( float x, float y );
	bool isGoal( float x, float y );
	void createObject( int type );
	void draw( float cameraX, float cameraY );
};

#endif // !_MAP_H_

