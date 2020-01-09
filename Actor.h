#ifndef _ACTOR_H_
#define _ACTOR_H_

class Actor {

protected:
	float x, y;	// キャラクタの座標．
	float size; // キャラクタの大きさ．
	float speed; // キャラクタの速度．
	float jumpPower; // キャラクタのジャンプ力．

public:
	virtual void move( float dx, float dy, char* map ) = 0;
	void draw() {

	}
};

#endif // !_ACTOR_H_

