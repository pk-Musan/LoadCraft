#include "Player.h"

Player::Player() {
	
}

std::tuple< int, int > Player::attack( int dx, int dy ) {
	/*
	
	*/
	return { ( int )( x / size ) + dx, ( int )( y / size ) + dy };
}