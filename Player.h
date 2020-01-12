#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "Actor.h"
#include "Block.h"
#include <vector>
#include <tuple>

enum PlayerImage {
	RIGHT,
	RIGHT_ATTACK_1,
	RIGHT_ATTACK_2,
	LEFT,
	LEFT_ATTACK_1,
	LEFT_ATTACK_2,

	PLAYER_MAX_INDEX
};

class Player : public Actor {
private:
	int power;
	std::vector<Block*> blocks;
	int selectedItemNum;
	int imageHandles[PLAYER_MAX_INDEX];
	
public:
	Player();
	~Player();
	void setPos( int sx, int sy );
	//std::tuple< int, int > setBlock( int dx, int dy );
	//std::tuple< int, int > attack( int dx, int dy );
	void getBlock( int maxDurability, int imageType );

	float getX() { return x; }
	float getY() { return y; }
	float getSize() { return size; }
	float getSpeed() { return speed; }
	float getJumpPower() { return jumpPower; }
	int getPower() { return power; }
	int getDirection() { return direction; }
	bool isAttacking() { return attackFlag; }

	float getTop() { return y - size * 0.5F + 3.0F; }
	float getBottom() { return y + size * 0.5F - 1.0F; }
	float getLeft() { return x - size * 0.5F + 4.0F; }
	float getRight() { return x + size * 0.5F - 1.0F - 3.0F; }

	void setDirection( int dir );
	void attack();
	void moveX( float dx );
	void moveY( float dy );
	void draw( float cameraX, float cameraY );
};

#endif // !_PLAYER_H_
