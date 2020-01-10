#ifndef _BLOCK_H_
#define _BLOCK_H_

#include "DxLib.h"

class Block {

protected:
	int x, y;
	int size;
	int durability;
	int imageType;

public:
	virtual void attacked( int power ) = 0;
	bool isBroken() { 
		if ( durability <= 0 ) return true;
		else return false;
	};
	int getImageType() {
		return imageType;
	};
	void draw( int imageHandle ) {
		DrawGraph( x - size / 2, y - size / 2, imageHandle, TRUE );
	};
};

#endif // !_BLOCK_H_
