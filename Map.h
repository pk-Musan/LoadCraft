#ifndef _MAP_H_
#define _MAP_H_

#include "Player.h"

class Map {
private:
	char *state;
	int width;
	int height;

public:
	Map( char *filename, Player *pl );
	void readFile( char *filename );
	void setPlayerPosition( Player *pl );
};

#endif // !_MAP_H_

