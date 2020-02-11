#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "Actor.h"
#include "Block.h"
#include <vector>

class Player : public Actor {
private:
	int power;
	std::vector<std::vector<Block*>> blocksList;
	int selectedItemNum;
	
public:
	Player();
	~Player();
	void setPos( int sx, int sy );

	float getX() { return x; }
	float getY() { return y; }
	float getSize() { return size; }
	float getSpeed() { return speed; }
	float getJumpPower() { return jumpPower; }
	int getPower() { return power; }
	int getDirection() { return direction; }
	bool isAttacking() { return attackFlag; }

	float getTop() { return y - size * 0.5F + 4.0F; }
	float getBottom() { return y + size * 0.5F - 1.0F; }
	float getLeft() { return x - size * 0.5F + 4.0F; }
	float getRight() { return x + size * 0.5F - 1.0F - 3.0F; }

	Block* putBlock( float x, float y );
	void getBlock( Block* block );
	void setDirection( int dir );
	void setMoveFlag( bool move );
	void changeSelectedItemNum();
	void attack();
	void moveX( float dx );
	void moveY( float dy );
	void draw( float cameraX, float cameraY );
};

#endif // !_PLAYER_H_
