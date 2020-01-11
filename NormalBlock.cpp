#include "NormalBlock.h"

NormalBlock::NormalBlock( int x, int y, int imageType ) {
	this->x = ( float )x * size + size * 0.5F;
	this->y = ( float )y * size + size * 0.5F;
	this->imageType = imageType;
	durability = 1;
}

void NormalBlock::attacked( int power ) {
	durability -= power;
	if ( durability < 0 ) durability = 0;
}