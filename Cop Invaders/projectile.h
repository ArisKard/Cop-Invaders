#pragma once
#include "gameobject.h"
#include "graphics.h"

class Projectile : public GameObject, public Collidable
{
	float proj_collision = 0.0f;
	bool active = true;

public:
	void update() override;
	void draw() override;
	void init() override;
	bool isActive() { return active; }
	void setPosX(float x) { pos_x = x; }
	void setPosY(float y) { pos_y = y; }
	void setSpeedto0() { speed = 0.0f; }
	void changeProjCollision() { proj_collision = 1.0f; }
	float getProjCollision() const { return proj_collision; }
	float getPosX() const { return pos_x; }
	float getPosY() const { return pos_y; }
	float getSize() const { return size; }
	Projectile(const class Game& mygame);
	~Projectile();
	Disk getCollisionHull() const override;
};