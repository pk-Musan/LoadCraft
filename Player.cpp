#include "DxLib.h"
#include "Player.h"

Player::Player() : power( 2 ) {
	imageHandle = LoadGraph( "Asset/player.png" );
	x = 0.0F;
	y = 0.0F;
	size = 32.0F;
	speed = 2.0F;
	jumpPower = 6.0F;
}

Player::~Player() {
	for ( Block* b : blocks ) {
		delete b;
		b = 0;
	}
	DeleteGraph( imageHandle );
}

void Player::setPos( int sx, int sy ) {
	x = ( float )( sx * size + size * 0.5F );
	y = ( float )( sy * size + size * 0.5F );
}

/*
std::tuple< int, int > Player::attack( int dx, int dy ) {
	return { ( int )( x / size ) + dx, ( int )( y / size ) + dy };
}

std::tuple< int, int > Player::setBlock( int dx, int dy ) {
	return { x + dx, y + dy };
}
*/

void Player::getBlock( Block* block ) {
	blocks.push_back( block );
}

void Player::moveX( float dx ) {
	x += dx;
}

void Player::moveY( float dy ) {
	y += dy;
}

void Player::draw() {
	DrawGraph( ( int )( x - size * 0.5F ), ( int )( y - size * 0.5F ), imageHandle, TRUE );
}