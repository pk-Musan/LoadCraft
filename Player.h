#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "Actor.h"

class Player : public Actor {
private:
		
public:
	Player();
	bool breakBlock( int x, int y );
};

#endif // !_PLAYER_H_
