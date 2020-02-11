#ifndef _MAP_H_
#define _MAP_H_

#include "Block.h"
#include "Warp.h"
#include "Arrow.h"
#include <vector>

class Map {
public:
	enum class MapObject {
		OBJ_SPACE,
		OBJ_BLOCK,
		OBJ_SPRING_BLOCK,
		OBJ_UNBREAKABLE_BLOCK,
		OBJ_PLAYER,
		OBJ_WARP,
		OBJ_GOAL,

		OBJ_UNKNOWN
	};
	const float CHIP_SIZE;

private:
	MapObject* mapObjects = 0;
	int width;
	int height;
	int startX, startY;
	int warpX, warpY;
	int goalX, goalY;
	std::vector<Arrow*> arrows;
	std::vector<Block*> blocks;
	std::vector<Warp*> warps;
	int warpAnimationCount;

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

	void setMapChip( float tx, float ty, MapObject o );
	void putBlock( Block* block );
	void eraseBlock();

	bool canPutBlock( float x, float y );
	bool hitCheck( float x, float y );
	bool isBlock( float x, float y );
	bool isWarp( float x, float y );
	bool isGoal( float x, float y );
	//void createObject( int type );
	void draw( float cameraX, float cameraY, bool stop );
};

#endif // !_MAP_H_

