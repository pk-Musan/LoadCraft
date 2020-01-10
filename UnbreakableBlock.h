#pragma once
#include "Block.h"

class UnbreakableBlock : public Block {

private:

public:
	UnbreakableBlock( int x, int y, int imageType );
	void attacked( int power );
};