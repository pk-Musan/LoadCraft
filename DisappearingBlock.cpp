#include "DisappearingBlock.h"

DisappearingBlock::DisappearingBlock( int x, int y, int imageType ) {
	visible = true;
	visibleCount = 0;

	this->x = ( float )x * size + size * 0.5F;
	this->y = ( float )y * size + size * 0.5F;
	this->imageType = imageType;
	durability = 100;
	maxDurability = durability;
	brokenAnimationCount = 0;
};

void DisappearingBlock::incrementVisibleCount() {
	visibleCount = ( visibleCount + 1 ) % 240;

	changeVisibility();
}

void DisappearingBlock::changeVisibility() {
	if ( visibleCount == 0 ) visible = true;
	else if ( visibleCount == 120 ) visible = false;
}

void DisappearingBlock::attacked( int power ) {
	durability = 100;
}