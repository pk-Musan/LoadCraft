#pragma once
#include "Block.h"

class SpringBlock :public Block {

private:
	float springCoefficient;
	float springPower;

public:
	SpringBlock( int x, int y, int imageType, float coefficient, float power );
	float getSpringCoefficient() { return springCoefficient; }
	float getSpringPower() { return springPower; }
	void attacked( int power );
};