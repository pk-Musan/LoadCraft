#include "Arrow.h"
#include "DxLib.h"
#include "Loader.h"

constexpr auto PI = 3.141592654;

Arrow::Arrow( int x, int y, int direction ) {
	this->x = x;
	this->y = y;
	this->direction = direction;
}

void Arrow::draw( float cameraX, float cameraY, float size ) {
	float aL, aR, aT, aB;
	aL = ( float )x * size;
	aR = ( float )( x + 1 ) * size - 1.0F;
	aT = ( float )y * size;
	aB = ( float )( y + 1 ) * size - 1.0F;

	if ( !( aR < cameraX || aL > cameraX + 640.0F - 1.0F || aB < cameraY || aT > cameraY + 480.0F - 1.0F ) ) {
		//DrawGraph( ( int )( x * size - cameraX ), ( int )( y * size - cameraY ), Loader::imageHandles[Loader::WARP_1 + animationCount / 20], TRUE );
		DrawRotaGraph( x * (int)size + ( (int)size / 2 ) - cameraX, y * (int)size + ( (int)size / 2 ) - cameraY, 1.0, ( double )direction * ( PI / 4 ), Loader::imageHandles[Loader::ARROW], TRUE );
	}
}