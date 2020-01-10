#ifndef _MAP_H_
#define _MAP_H_

#include "Block.h"
//#include "Player.h"
#include <vector>
#include <tuple>

typedef enum {
	OBJ_SPACE,
	OBJ_BLOCK,
	OBJ_BREAKABLE_BLOCK,
	OBJ_PLAYER,

	OBJ_UNKNOWN
} MapObject;

class Map {
private:
	//int* state = 0;

	MapObject* mapObjects = 0;
	int width;
	int height;
	int startX;
	int startY;
	std::vector<Block*> blocks;
	int imageHandles[2];

public:
	Map( const char* stageData, int fileSize );
	void setSize( const char* stageData, int fileSeize );
	std::tuple< int, int > getStartPos();
	void update( int x, int y, int type );
	bool hitCheck( int x, int y );
	void createObject( int type );
	void draw();
	
};

#endif // !_MAP_H_

