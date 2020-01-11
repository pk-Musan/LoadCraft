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

class Map {
private:
	MapObject* mapObjects = 0;
	int width;
	int height;
	int startX, startY;
	int goalX, goalY;
	std::vector<Block*> blocks;
	int imageHandles[2];

public:
	const float CHIP_SIZE;

public:
	Map( const char* stageData, int fileSize );
	~Map();
	void setSize( const char* stageData, int fileSeize );
	int getStartX() { return startX; }
	int getStartY() { return startY; }
	MapObject getMapChip( float tx, float ty );
	Block* getBlock( float x, float y );
	void eraseBlock( float x, float y );
	bool hitCheck( float x, float y );
	bool isBlock( float x, float y );
	bool isGoal( float x, float y );
	void createObject( int type );
	void draw();
};

#endif // !_MAP_H_

