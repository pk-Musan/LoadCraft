#include "SpringBlock.h"

SpringBlock::SpringBlock( int x, int y, int imageType, float coefficient, float power ) {
	springCoefficient = coefficient;
	springPower = power;

	this->x = ( float )x * size + size * 0.5F;
	this->y = ( float )y * size + size * 0.5F;
	this->imageType = imageType;
	durability = 4;
	maxDurability = durability;
	brokenAnimationCount = 0;
}

void SpringBlock::attacked( int power ) {
	durability -= power;
	if ( durability < 0 ) durability = 0;
}