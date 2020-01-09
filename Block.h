#ifndef _BLOCK_H_
#define _BLOCK_H_

class Block {

private:
	float x, y;
	float size;
	//int type;
	int durability;

public:
	Block( int x, int y );
	void attacked( int power );
	bool isBroken();
	void draw();
};

#endif // !_BLOCK_H_
