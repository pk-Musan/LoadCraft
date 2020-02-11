#pragma once

class Warp {

private:
	int x, y;
	int animationCount;

public:
	Warp( int x, int y );
	int getX() { return x; }
	int getY() { return y; }
	int getAnimationCount() { return animationCount; }
	void incrementAnimationCount();
	void draw( float cameraX, float cameraY, float size );
};