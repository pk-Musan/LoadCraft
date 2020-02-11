#pragma once
#include "Block.h"

class DisappearingBlock : public Block {

private:
	bool visible;
	int visibleCount;

public:
	DisappearingBlock( int x, int y, int imageType );
	void incrementVisibleCount();
	void changeVisibility();
	void attacked( int power );

	bool isVisible() { return visible; }
};