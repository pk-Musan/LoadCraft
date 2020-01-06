#include "Player.h"

Player::Player() : power(2) {
}

void Player::setPos( int sx, int sy ) {
	x = sx;
	y = sy;
}

std::tuple< int, int > Player::attack( int dx, int dy ) {
	return { ( int )( x / size ) + dx, ( int )( y / size ) + dy };
}

std::tuple< int, int > Player::setBlock( int dx, int dy ) {
	return { x + dx, y + dy };
}

void Player::getBlock( Block block ) {
	blocks.push_back( block );
}