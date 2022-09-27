#pragma once
#include "gameobject.h"
#include "graphics.h"

class Bullet : public GameObject, public Collidable
{
	bool active = true;

public:
	void update() override;
	void draw() override;
	void init() override;
	bool isActive() { return active; }
	void setPosX(float x) { pos_x = x; }
	void setPosY(float y) { pos_y = y; }
	float getPosX() const { return pos_x; }
	float getPosY() const { return pos_y; }
	float getSize() const { return size; }
	Bullet(const class Game& mygame);
	~Bullet();
	Disk getCollisionHull() const override;
};