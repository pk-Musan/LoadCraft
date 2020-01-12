#ifndef _BLOCK_H_
#define _BLOCK_H_

#include "DxLib.h"

class Block {

protected:
	float x, y;
	float size = 32.0F;
	int durability;
	int maxDurability;
	int imageType;

public:
	int brokenAnimationCount;

public:
	virtual void attacked( int power ) = 0;
	float getX() { return x; }
	float getY() { return y; }
	int getMaxDurability() { return maxDurability; }
	int getImageType() { return imageType; }
	void setPos( float x, float y ) {
		this->x = x;
		this->y = y;
	}
	bool isBroken() {
		if ( durability <= 0 ) return true;
		else return false;
	};
	void draw( float cameraX, float cameraY, int imageHandle ) {
		float X, Y;
		X = x - cameraX;
		Y = y - cameraY;

		DrawGraph( ( int )( X - size * 0.5F ), ( int )( Y - size * 0.5F ), imageHandle, TRUE );
	};
};

#endif // !_BLOCK_H_
