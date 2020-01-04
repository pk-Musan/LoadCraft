#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "Actor.h"
#include "Map.h"
#include <tuple>

class Player : public Actor {
private:
	
public:
	Player();
	std::tuple< int, int > attack( int dx, int dy );
};

#endif // !_PLAYER_H_
