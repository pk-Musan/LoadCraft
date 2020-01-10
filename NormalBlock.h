#pragma once
#include "Block.h"

class NormalBlock : public Block {

private:

public:
	NormalBlock( int x, int y, int imageType );
	void attacked( int power );
};