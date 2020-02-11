#include "Warp.h"
#include "Loader.h"
#include "DxLib.h"

Warp::Warp( int x, int y ) {
	this->x = x;
	this->y = y;

	animationCount = 0;
}

void Warp::incrementAnimationCount() {
	animationCount = ( animationCount + 1 ) % 40;
}

void Warp::draw( float cameraX, float cameraY, float size ) {
	float wL, wR, wT, wB;
	wL = ( float )x * size;
	wR = ( float )( x + 1 ) * size - 1.0F;
	wT = ( float )y * size;
	wB = ( float )( y + 1 ) * size - 1.0F;

	if ( !( wR < cameraX || wL > cameraX + 640.0F - 1.0F || wB < cameraY || wT > cameraY + 480.0F - 1.0F ) ) {
		DrawGraph( ( int )( x * size - cameraX ), ( int )( y * size - cameraY ), Loader::imageHandles[Loader::WARP_1 + animationCount / 20], TRUE );
	}
}