#ifndef _MAP_H_
#define _MAP_H_

#include "Player.h"
#include <tuple>

class Map {
private:
	int *state = 0;
	int width;
	int height;
	int startX;
	int startY;

public:
	Map( char *filename );
	std::tuple< int, int > getStartPos();
	void update( int x, int y, int type );
	bool hitCheck( int x, int y );
	void createObject( int type );
};

#endif // !_MAP_H_

