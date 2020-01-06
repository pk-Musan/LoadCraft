#ifndef _GAME_H_
#define _GAME_H_

#include "Player.h"
#include "Map.h"

class Game {
private:
	Player *player = 0;
	std::vector<Map> maps;

public:
	void init();
	void input();
	void loop();
	void draw();
};

#endif // !_GAME_H_
