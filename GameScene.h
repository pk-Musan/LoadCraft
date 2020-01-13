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
	float dx, dy; // x, y�����̈ړ���
	float jumpSpeed; // y�����̑���
	float g; // �d�͉����x

	Map* map = 0; // �}�b�v
	Block* targetBlock = 0; // �v���C���[���U�������u���b�N

	float cameraX, cameraY; // ��ʃX�N���[���p�̃J�����ʒu

	int font[2]; // �X�e�[�W�N���A���Ɏg���t�H���g
	int finishCount; // �S�[�����B������J�E���g�J�n�C����̃t���[�����o�߂ŉ�ʑJ�ڂ��邽�߂̃J�E���g

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
