#pragma once
#include "gameobject.h"
#include "config.h"
#include "graphics.h"

class BonusEnemy : public GameObject, public Collidable
{
	float collision = 0.0f;
	bool active = true;
	bool left_to_right;

public:
	void update() override;
	void draw() override;
	void init() override {};
	void init(bool ltr);
	bool isActive() { return active; }
	float getPosX() { return pos_x; }
	float getPosY() { return pos_y; }
	void changeCollision() { collision = 1.0f; }
	float getCollision() const { return collision; }
	BonusEnemy(const Game& mygame, bool ltr);
	~BonusEnemy();
	Disk getCollisionHull() const override;
};