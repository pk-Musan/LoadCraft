#pragma once
#include "Block.h"

class NormalBlock : public Block {

private:

public:
	NormalBlock( int x, int y, int maxDurability, int imageType );
	void attacked( int power );
};