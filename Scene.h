#pragma once

class Scene {
private:
	int image;

public:
	virtual void init() = 0;
	virtual Scene* update() = 0;
	virtual void draw() = 0;
};