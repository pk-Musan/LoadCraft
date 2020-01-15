#ifndef _ACTOR_H_
#define _ACTOR_H_

#include "DxLib.h"

class Actor {

protected:
	float x, y;	// �L�����N�^�̍��W�D
	float dx, dy;
	float size; // �L�����N�^�̑傫���D
	float speed; // �L�����N�^�̑��x�D
	float jumpPower; // �L�����N�^�̃W�����v�́D

	int direction;
	bool attackFlag;
	int attackAnimationCount;
	bool moveFlag;
	int moveAnimationCount;

public:
};

#endif // !_ACTOR_H_

