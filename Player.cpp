#include "DxLib.h"
#include "Player.h"

Player::Player( int imageHandle ) : power( 2 ) {
	this->imageHandle = imageHandle;
	x = y = 0.0F;
	size = 32.0F;
	speed = 0.0F;
	jumpPower = 0.0F;
}

void Player::setPos( int sx, int sy ) {
	x = ( float )( sx * size + size * 0.5F );
	y = ( float )( sy * size + size * 0.5F );
}

std::tuple< int, int > Player::attack( int dx, int dy ) {
	return { ( int )( x / size ) + dx, ( int )( y / size ) + dy };
}

std::tuple< int, int > Player::setBlock( int dx, int dy ) {
	return { x + dx, y + dy };
}

void Player::getBlock( Block* block ) {
	blocks.push_back( block );
}

void Player::move( float dx, float dy ) {

}

void Player::draw() {
	DrawGraph( ( int )( x - size * 0.5F ), ( int )( y - size * 0.5F ), imageHandle, TRUE );
}