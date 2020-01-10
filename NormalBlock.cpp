#include "NormalBlock.h"

NormalBlock::NormalBlock( int x, int y, int imageType ) {
	this->size = 32;
	this->x = x * size + size / 2;
	this->y = y * size + size / 2;
	this->imageType = imageType;
	durability = 1;
}

void NormalBlock::attacked( int power ) {
	durability -= power;
	if ( durability < 0 ) durability = 0;
}