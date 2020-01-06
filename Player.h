#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "Actor.h"
#include "Map.h"
#include "Block.h"
#include <vector>
#include <tuple>

class Player : public Actor {
private:
	int power;
	std::vector<Block> blocks;
	
public:
	Player();
	void setPos( int sx, int sy );
	std::tuple< int, int > setBlock( int dx, int dy );
	std::tuple< int, int > attack( int dx, int dy );
	void getBlock( Block block );
	int getPower() { return power; }
};

#endif // !_PLAYER_H_
