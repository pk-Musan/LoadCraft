#ifndef _GAMESCENE_H_
#define _GAMESCENE_H_

#include "Scene.h"
#include "Player.h"
#include "Map.h"

#include <string>

class GameScene : public Scene {
private:
	Player* player = 0;
	std::vector<Map*> maps;
	Map* map = 0;
	// int currentMapNum;

public:
	GameScene( const char *filename );
	void init();
	Scene* update();
	void draw();
	void input();
	//void readFile( const char** buffer, int* size, const char* filename );
	void readFile( std::string** buffer, int* size, const char* filename );
};

#endif // !_GAMESCENE_H_
