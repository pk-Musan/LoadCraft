#ifndef _GAMESCENE_H_
#define _GAMESCENE_H_

#include "Scene.h"
#include "Player.h"
#include "Map.h"

#include <string>

class GameScene : public Scene {
private:
	Player* player = 0;
	float dx, dy; // x, y•ûŒü‚ÌˆÚ“®—Ê
	float jumpSpeed; // y•ûŒü‚Ì‘¬‚³
	float g; // d—Í‰Á‘¬“x
	//std::vector<Map*> maps;
	Map* map = 0;
	// int currentMapNum;

public:
	GameScene( const char *filename );
	virtual ~GameScene();
	void init();
	Scene* update();
	void draw();
	void charaMove( float charaTop, float charaBottom, float charaLeft, float charaRight, float chipSize );
	void attackBlock( float charaX, float charaY,  float chipSize );
	//void readFile( const char** buffer, int* size, const char* filename );
	void readFile( std::string** buffer, const char* filename );
};

#endif // !_GAMESCENE_H_
