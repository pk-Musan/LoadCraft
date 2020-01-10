#ifndef _ACTOR_H_
#define _ACTOR_H_

#include "DxLib.h"

class Actor {

protected:
	float x, y;	// �L�����N�^�̍��W�D
	float size; // �L�����N�^�̑傫���D
	float speed; // �L�����N�^�̑��x�D
	float jumpPower; // �L�����N�^�̃W�����v�́D

public:
	virtual void move( float dx, float dy ) = 0;
};

#endif // !_ACTOR_H_

