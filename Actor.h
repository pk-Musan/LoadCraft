#ifndef _ACTOR_H_
#define _ACTOR_H_

#include "DxLib.h"

class Actor {

protected:
	float x, y;	// キャラクタの座標．
	float dx, dy;
	float size; // キャラクタの大きさ．
	float speed; // キャラクタの速度．
	float jumpPower; // キャラクタのジャンプ力．

	int direction;
	bool attackFlag;
	int attackAnimationCount;
	bool moveFlag;
	int moveAnimationCount;

public:
};

#endif // !_ACTOR_H_

