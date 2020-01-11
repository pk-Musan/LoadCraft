#ifndef _BLOCK_H_
#define _BLOCK_H_

#include "DxLib.h"

class Block {

protected:
	float x, y;
	float size = 32.0F;
	int durability;
	int imageType;

public:
	virtual void attacked( int power ) = 0;
	float getX() { return x; }
	float getY() { return y; }
	bool isBroken() { 
		if ( durability <= 0 ) return true;
		else return false;
	};
	int getImageType() {
		return imageType;
	};
	void draw( int imageHandle ) {
		DrawGraph( ( int )( x - size * 0.5F ), ( int )( y - size * 0.5F ), imageHandle, TRUE );
	};
};

#endif // !_BLOCK_H_
