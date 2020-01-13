#ifndef _GAMESCENE_H_
#define _GAMESCENE_H_

#include "Scene.h"
#include "Player.h"
#include "Map.h"
#include "Block.h"

#include <string>

class GameScene : public Scene {
private:
	Player* player = 0;
	float dx, dy; // x, y方向の移動量
	float jumpSpeed; // y方向の速さ
	float g; // 重力加速度

	Map* map = 0; // マップ
	Block* targetBlock = 0; // プレイヤーが攻撃したブロック

	float cameraX, cameraY; // 画面スクロール用のカメラ位置

	int font[2]; // ステージクリア時に使うフォント
	int finishCount; // ゴール到達時からカウント開始，特定のフレーム数経過で画面遷移するためのカウント

	bool readError;

public:
	GameScene( const char *filename );
	virtual ~GameScene();
	void init();
	Scene* update();
	void draw();
	Block* attackBlock( float charaX, float charaY, float chipSize );
	void putBlock( float charaX, float charaY, float charaLeft, float charaRight, float charaTop, float charaBottom, float chipSize );
	void charaMove( float charaLeft, float charaRight, float charaTop, float charaBottom, float chipSize );
	void cameraMove();
	void readFile( std::string** buffer, const char* filename );
};

#endif // !_GAMESCENE_H_
