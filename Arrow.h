#pragma once

class Arrow {

private:
	int x, y;
	int direction;

public:
	Arrow( int x, int y, int direction );
	void draw( float cameraX, float cameraY, float size );
};