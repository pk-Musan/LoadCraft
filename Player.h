#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "Actor.h"
#include "Block.h"
#include <vector>
#include <tuple>

class Player : public Actor {
private:
	int power;
	std::vector<Block*> blocks;
	int imageHandle;
	
public:
	Player();
	~Player();
	void setPos( int sx, int sy );
	//std::tuple< int, int > setBlock( int dx, int dy );
	//std::tuple< int, int > attack( int dx, int dy );
	void getBlock( Block* block );

	float getX() { return x; }
	float getY() { return y; }
	float getSize() { return size; }
	float getSpeed() { return speed; }
	float getJumpPower() { return jumpPower; }
	int getPower() { return power; }

	float getTop() { return y - size * 0.5F + 3.0F; }
	float getBottom() { return y + size * 0.5F - 1.0F; }
	float getLeft() { return x - size * 0.5F + 4.0F; }
	float getRight() { return x + size * 0.5F - 1.0F - 3.0F; }

	void moveX( float dx );
	void moveY( float dy );
	void draw();
};

#endif // !_PLAYER_H_
