#include "UnbreakableBlock.h"

UnbreakableBlock::UnbreakableBlock( int x, int y, int imageType ) {
	this->size = 32;
	this->x = x * size + size / 2;
	this->y = y * size + size / 2;
	this->imageType = imageType;
	durability = 100;
}

void UnbreakableBlock::attacked( int power ) {
	durability = 100;
}