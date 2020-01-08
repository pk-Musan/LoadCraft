#ifndef _GAMESCENE_H_
#define _GAMESCENE_H_

#include "Scene.h"
#include "Player.h"
#include "Map.h"

class GameScene : public Scene {
private:
	Player *player = 0;
	std::vector<Map> maps;

public:
	GameScene( char *filename );
	void init();
	Scene* update();
	void draw();
	void input();
};

#endif // !_GAMESCENE_H_
