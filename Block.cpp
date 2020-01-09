#include "Block.h"

Block::Block( int x, int y ) {
	size = 32.0f;
	this->x = x * size + size / 2;
	this->y = y * size + size / 2;
	durability = 2;
}

void Block::attacked( int power ) {
	durability -= power;
	
	if ( durability < 0 ) durability = 0;
}

bool Block::isBroken() {
	if ( durability <= 0 ) return true;
	else return false;
}

void Block::draw() {

}