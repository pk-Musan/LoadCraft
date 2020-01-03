#include "Map.h"

Map::Map( char *filename, Player *pl ) {
	readFile( filename );
	setPlayerPosition( pl );
}

void Map::readFile( char *filename ) {
	// state ‚ð–„‚ß‚Ä‚¢‚­
}

void Map::setPlayerPosition( Player *pl ) {
	/*
	int x, y;
	for ( ) {
		if ( state[i] == 1 ) {
			x = ;
			y = ;
		}
	}

	pl->setX( x );
	pl->setY( y );
	*/
}