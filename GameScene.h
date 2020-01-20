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

	const char* filename;
	Map* map = 0; // �}�b�v
	Block* targetBlock = 0; // �v���C���[���U�������u���b�N

	float cameraX, cameraY; // ��ʃX�N���[���p�̃J�����ʒu(������W)
	int targetCursorX, targetCursorY; // �^�[�Q�b�g�J�[�\���̈ʒu�i������W�j

	//int font[3]; // �X�e�[�W�N���A���Ɏg���t�H���g
	int finishCount; // �S�[�����B������J�E���g�J�n�C����̃t���[�����o�߂ŉ�ʑJ�ڂ��邽�߂̃J�E���g
	int warpCount;

	bool pause;
	bool operation;
	int pauseCursor;
	int operationPage;
	int cursorAnimationCount;

	bool readError;

public:
	GameScene( const char *filename );
	virtual ~GameScene();
	void init();
	Scene* update();
	void draw();
	Block* attackBlock( float charaX, float charaY, float chipSize );
	void putBlock( float charaX, float charaY, float charaLeft, float charaRight, float charaTop, float charaBottom, float chipSize );
	void moveChara( float charaLeft, float charaRight, float charaTop, float charaBottom, float chipSize );
	void moveCamera();
	void moveTargetCursor();
	void readFile( std::string** buffer, const char* filename );
};

#endif // !_GAMESCENE_H_
