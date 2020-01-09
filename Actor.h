#ifndef _ACTOR_H_
#define _ACTOR_H_

class Actor {

protected:
	float x, y;	// �L�����N�^�̍��W�D
	float size; // �L�����N�^�̑傫���D
	float speed; // �L�����N�^�̑��x�D
	float jumpPower; // �L�����N�^�̃W�����v�́D

public:
	virtual void move( float dx, float dy, char* map ) = 0;
	void draw() {

	}
};

#endif // !_ACTOR_H_

