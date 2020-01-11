#include "UnbreakableBlock.h"

UnbreakableBlock::UnbreakableBlock( int x, int y, int imageType ) {
	this->x = ( float )x * size + size * 0.5F;
	this->y = ( float )y * size + size * 0.5F;
	this->imageType = imageType;
	durability = 100;
}

void UnbreakableBlock::attacked( int power ) {
	durability = 100;
}